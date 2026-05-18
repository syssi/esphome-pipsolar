#include "pipsolar.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "pipsolar_select.h"

namespace esphome::pi18 {

static const char *const TAG = "pi18";

void Pipsolar::setup() {
  this->state_ = STATE_IDLE;
  this->command_start_millis_ = 0;
}

void Pipsolar::empty_uart_buffer_() {
  uint8_t buf[64];
  size_t avail;
  while ((avail = this->available()) > 0) {
    if (!this->read_array(buf, std::min(avail, sizeof(buf)))) {
      break;
    }
  }
}

void Pipsolar::loop() {
  if (this->state_ == STATE_IDLE) {
    this->empty_uart_buffer_();

    if (this->send_next_command_()) {
      return;
    }

    if (this->send_next_poll_()) {
      return;
    }

    return;
  }
  if (this->state_ == STATE_COMMAND_COMPLETE) {
    if (this->check_incoming_length_(4)) {
      if (this->check_incoming_crc_()) {
        if (this->read_buffer_[1] == 'A' && this->read_buffer_[2] == 'C' && this->read_buffer_[3] == 'K') {
          ESP_LOGD(TAG, "command successful");
        } else {
          ESP_LOGD(TAG, "command not successful");
        }
        this->command_queue_[this->command_queue_position_] = std::string("");
        this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
        this->state_ = STATE_IDLE;
      } else {
        this->command_queue_[this->command_queue_position_] = std::string("");
        this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
        this->state_ = STATE_IDLE;
      }
    } else {
      ESP_LOGD(TAG, "command %s response length not OK: with length %zu",
               this->command_queue_[this->command_queue_position_].c_str(), this->read_pos_);
      this->command_queue_[this->command_queue_position_] = std::string("");
      this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
      this->state_ = STATE_IDLE;
    }
  }

  if (this->state_ == STATE_POLL_CHECKED) {
    ESP_LOGD(TAG, "poll %s decode", this->enabled_polling_commands_[this->last_polling_command_].command);
    this->handle_poll_response_(this->enabled_polling_commands_[this->last_polling_command_].identifier,
                                (const char *) this->read_buffer_);
    this->state_ = STATE_IDLE;
    return;
  }

  if (this->state_ == STATE_POLL_COMPLETE) {
    if (this->check_incoming_crc_()) {
      if (this->read_buffer_[0] == '(' && this->read_buffer_[1] == 'N' && this->read_buffer_[2] == 'A' &&
          this->read_buffer_[3] == 'K') {
        ESP_LOGD(TAG, "poll %s NACK", this->enabled_polling_commands_[this->last_polling_command_].command);
        this->handle_poll_error_(this->enabled_polling_commands_[this->last_polling_command_].identifier);
        this->state_ = STATE_IDLE;
        return;
      }
      this->enabled_polling_commands_[this->last_polling_command_].needs_update = false;
      this->state_ = STATE_POLL_CHECKED;
      return;
    } else {
      this->handle_poll_error_(this->enabled_polling_commands_[this->last_polling_command_].identifier);
      this->state_ = STATE_IDLE;
    }
  }

  if (this->state_ == STATE_COMMAND || this->state_ == STATE_POLL) {
    size_t avail = this->available();
    while (avail > 0) {
      uint8_t buf[64];
      size_t to_read = std::min(avail, sizeof(buf));
      if (!this->read_array(buf, to_read)) {
        break;
      }
      avail -= to_read;
      bool done = false;
      for (size_t i = 0; i < to_read; i++) {
        uint8_t byte = buf[i];

        if (this->read_pos_ >= PIPSOLAR_READ_BUFFER_LENGTH - 1) {
          this->read_pos_ = 0;
          this->empty_uart_buffer_();
          ESP_LOGW(TAG, "response data too long, discarding.");
          done = true;
          break;
        }
        this->read_buffer_[this->read_pos_] = byte;
        this->read_pos_++;

        if (byte == 0x0D) {
          this->read_buffer_[this->read_pos_] = 0;
          this->empty_uart_buffer_();
          if (this->state_ == STATE_POLL) {
            this->state_ = STATE_POLL_COMPLETE;
          }
          if (this->state_ == STATE_COMMAND) {
            this->state_ = STATE_COMMAND_COMPLETE;
          }
          done = true;
          break;
        }
      }
      if (done) {
        break;
      }
    }
  }
  if (this->state_ == STATE_COMMAND) {
    if (millis() - this->command_start_millis_ > esphome::pi18::Pipsolar::COMMAND_TIMEOUT) {
      const char *command = this->command_queue_[this->command_queue_position_].c_str();
      this->command_start_millis_ = millis();
      ESP_LOGD(TAG, "command %s timeout", command);
      this->command_queue_[this->command_queue_position_] = std::string("");
      this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
      this->state_ = STATE_IDLE;
      return;
    }
  }
  if (this->state_ == STATE_POLL) {
    if (millis() - this->command_start_millis_ > esphome::pi18::Pipsolar::COMMAND_TIMEOUT) {
      ESP_LOGD(TAG, "poll %s timeout", this->enabled_polling_commands_[this->last_polling_command_].command);
      this->handle_poll_error_(this->enabled_polling_commands_[this->last_polling_command_].identifier);
      this->state_ = STATE_IDLE;
    }
  }
}

uint8_t Pipsolar::check_incoming_length_(uint8_t length) {
  if (this->read_pos_ >= 3 && this->read_pos_ - 3 == length) {
    return 1;
  }
  return 0;
}

uint8_t Pipsolar::check_incoming_crc_() {
  if (this->read_pos_ < 3)
    return 0;
  uint16_t crc16;
  crc16 = this->pipsolar_crc_(read_buffer_, read_pos_ - 3);
  if (((uint8_t) ((crc16) >> 8)) == read_buffer_[read_pos_ - 3] &&
      ((uint8_t) ((crc16) &0xff)) == read_buffer_[read_pos_ - 2]) {
    ESP_LOGD(TAG, "CRC OK");
    read_buffer_[read_pos_ - 1] = 0;
    read_buffer_[read_pos_ - 2] = 0;
    read_buffer_[read_pos_ - 3] = 0;
    return 1;
  }
  ESP_LOGD(TAG, "CRC NOK expected: %X %X but got: %X %X", ((uint8_t) ((crc16) >> 8)), ((uint8_t) ((crc16) &0xff)),
           read_buffer_[read_pos_ - 3], read_buffer_[read_pos_ - 2]);
  return 0;
}

bool Pipsolar::send_next_command_() {
  uint16_t crc16;
  if (!this->command_queue_[this->command_queue_position_].empty()) {
    const char *command = this->command_queue_[this->command_queue_position_].c_str();
    uint8_t byte_command[32];
    size_t length = this->command_queue_[this->command_queue_position_].length();
    if (length > sizeof(byte_command)) {
      ESP_LOGE(TAG, "Command too long: %zu", length);
      this->command_queue_[this->command_queue_position_].clear();
      return false;
    }
    for (size_t i = 0; i < length; i++) {
      byte_command[i] = (uint8_t) this->command_queue_[this->command_queue_position_].at(i);
    }
    this->state_ = STATE_COMMAND;
    this->command_start_millis_ = millis();
    this->empty_uart_buffer_();
    this->read_pos_ = 0;
    crc16 = this->pipsolar_crc_(byte_command, length);
    this->write_str(command);
    this->write(((uint8_t) ((crc16) >> 8)));
    this->write(((uint8_t) ((crc16) &0xff)));
    this->write(0x0D);
    ESP_LOGD(TAG, "Sending command from queue: %s with length %zu", command, length);
    return true;
  }
  return false;
}

bool Pipsolar::send_next_poll_() {
  uint16_t crc16;
  for (uint8_t i = 0; i < POLLING_COMMANDS_MAX; i++) {
    this->last_polling_command_ = (this->last_polling_command_ + 1) % POLLING_COMMANDS_MAX;
    if (this->enabled_polling_commands_[this->last_polling_command_].length == 0) {
      continue;
    }
    if (!this->enabled_polling_commands_[this->last_polling_command_].needs_update) {
      continue;
    }
    this->state_ = STATE_POLL;
    this->command_start_millis_ = millis();
    this->empty_uart_buffer_();
    this->read_pos_ = 0;
    crc16 = this->pipsolar_crc_(this->enabled_polling_commands_[this->last_polling_command_].command,
                                this->enabled_polling_commands_[this->last_polling_command_].length);
    this->write_array(this->enabled_polling_commands_[this->last_polling_command_].command,
                      this->enabled_polling_commands_[this->last_polling_command_].length);
    this->write(((uint8_t) ((crc16) >> 8)));
    this->write(((uint8_t) ((crc16) &0xff)));
    this->write(0x0D);
    ESP_LOGD(TAG, "Sending polling command: %s with length %d",
             this->enabled_polling_commands_[this->last_polling_command_].command,
             this->enabled_polling_commands_[this->last_polling_command_].length);
    return true;
  }
  return false;
}

void Pipsolar::queue_command(const std::string &command) {
  uint8_t next_position = command_queue_position_;
  for (uint8_t i = 0; i < COMMAND_QUEUE_LENGTH; i++) {
    uint8_t testposition = (next_position + i) % COMMAND_QUEUE_LENGTH;
    if (command_queue_[testposition].empty()) {
      command_queue_[testposition] = command;
      ESP_LOGD(TAG, "Command queued successfully: %s at position %d", command.c_str(), testposition);
      return;
    }
  }
  ESP_LOGD(TAG, "Command queue full dropping command: %s", command.c_str());
}

void Pipsolar::handle_poll_response_(ENUMPollingCommand polling_command, const char *message) {
  switch (polling_command) {
    case POLLING_P007PIRI:
      handle_p007piri_(message);
      break;
    case POLLING_P005GS:
      handle_p005gs_(message);
      break;
    case POLLING_P006MOD:
      handle_p006mod_(message);
      break;
    case POLLING_P007FLAG:
      handle_p007flag_(message);
      break;
    case POLLING_P005FWS:
      handle_p005fws_(message);
      break;
    case POLLING_P005ET:
      handle_p005et_(message);
      break;
    case POLLING_P007PGS0:
      handle_p007pgs0_(message);
      break;
    default:
      break;
  }
}

void Pipsolar::handle_poll_error_(ENUMPollingCommand polling_command) {
  this->handle_poll_response_(polling_command, "");
}

void Pipsolar::handle_p005gs_(const char *message) {
  if (this->last_p005gs_)
    this->last_p005gs_->publish_state(message);

  size_t pos = 0;
  skip_start_(message, &pos);

  read_sensor_(message, &pos, this->grid_voltage_, 0.1f);
  read_sensor_(message, &pos, this->grid_frequency_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_voltage_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_frequency_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_apparent_power_);
  read_sensor_(message, &pos, this->ac_output_active_power_);
  read_sensor_(message, &pos, this->output_load_percent_);
  read_sensor_(message, &pos, this->battery_voltage_, 0.1f);
  read_sensor_(message, &pos, this->battery_voltage_scc_, 0.1f);
  read_sensor_(message, &pos, this->battery_voltage_scc2_, 0.1f);
  read_sensor_(message, &pos, this->battery_discharge_current_, -1.0f);
  read_sensor_(message, &pos, this->battery_charging_current_);
  read_sensor_(message, &pos, this->battery_capacity_percent_);
  read_sensor_(message, &pos, this->inverter_heat_sink_temperature_);
  read_sensor_(message, &pos, this->mppt1_charger_temperature_);
  read_sensor_(message, &pos, this->mppt2_charger_temperature_);
  read_sensor_(message, &pos, this->pv1_input_power_);
  read_sensor_(message, &pos, this->pv2_input_power_);
  read_sensor_(message, &pos, this->pv1_input_voltage_, 0.1f);
  read_sensor_(message, &pos, this->pv2_input_voltage_, 0.1f);

  const int config_state = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int mppt1_status = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int mppt2_status = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int load_conn = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int bat_dir = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int dc_ac_dir = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int line_dir = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);

  this->publish_binary_sensor_(config_state != 0, this->setting_value_configuration_state_);
  this->publish_mppt_status_(this->mppt1_charger_status_, mppt1_status);
  this->publish_mppt_status_(this->mppt2_charger_status_, mppt2_status);

  if (this->load_connection_)
    this->load_connection_->publish_state(load_conn == 1 ? "connect" : "disconnect");

  if (this->battery_power_direction_) {
    switch (bat_dir) {
      case 0:
        this->battery_power_direction_->publish_state("donothing");
        break;
      case 1:
        this->battery_power_direction_->publish_state("charge");
        break;
      case 2:
        this->battery_power_direction_->publish_state("discharge");
        break;
      default:
        break;
    }
  }
  if (this->dc_ac_power_direction_) {
    switch (dc_ac_dir) {
      case 0:
        this->dc_ac_power_direction_->publish_state("donothing");
        break;
      case 1:
        this->dc_ac_power_direction_->publish_state("AC-DC");
        break;
      case 2:
        this->dc_ac_power_direction_->publish_state("DC-AC");
        break;
      default:
        break;
    }
  }
  this->publish_direction_(this->line_power_direction_, line_dir);
}

void Pipsolar::handle_p007piri_(const char *message) {
  if (this->last_p007piri_)
    this->last_p007piri_->publish_state(message);

  size_t pos = 0;
  skip_start_(message, &pos);

  read_sensor_(message, &pos, this->grid_rating_voltage_, 0.1f);
  read_sensor_(message, &pos, this->grid_rating_current_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_rating_voltage_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_rating_frequency_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_rating_current_, 0.1f);
  read_sensor_(message, &pos, this->ac_output_rating_apparent_power_);
  read_sensor_(message, &pos, this->ac_output_rating_active_power_);
  read_sensor_(message, &pos, this->battery_rating_voltage_, 0.1f);
  read_sensor_(message, &pos, this->battery_recharge_voltage_, 0.1f);
  read_sensor_(message, &pos, this->battery_redischarge_voltage_, 0.1f);

  const int bat_under_v_raw = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  if (this->battery_under_voltage_)
    this->battery_under_voltage_->publish_state(bat_under_v_raw * 0.1f);

  read_sensor_(message, &pos, this->battery_bulk_voltage_, 0.1f);
  read_sensor_(message, &pos, this->battery_float_voltage_, 0.1f);

  const int bat_type = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int max_ac_charging_i = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int max_charging_i = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int input_v_range = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int out_src_priority = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int charger_src_priority = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int parallel_max = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int machine_type = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int topology = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int output_mode = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int solar_priority = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  const int mppt_string = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);

  if (this->battery_type_)
    this->battery_type_->publish_state(bat_type);
  if (this->current_max_ac_charging_current_)
    this->current_max_ac_charging_current_->publish_state(max_ac_charging_i);
  if (this->current_max_charging_current_)
    this->current_max_charging_current_->publish_state(max_charging_i);
  if (this->input_voltage_range_)
    this->input_voltage_range_->publish_state(input_v_range);
  if (this->output_source_priority_)
    this->output_source_priority_->publish_state(out_src_priority);
  if (this->charger_source_priority_)
    this->charger_source_priority_->publish_state(charger_src_priority);
  if (this->parallel_max_num_)
    this->parallel_max_num_->publish_state(parallel_max);
  if (this->machine_type_)
    this->machine_type_->publish_state(machine_type);
  if (this->topology_)
    this->topology_->publish_state(topology);
  if (this->output_mode_)
    this->output_mode_->publish_state(output_mode);
  if (this->solar_power_priority_)
    this->solar_power_priority_->publish_state(solar_priority);
  if (this->mppt_string_)
    this->mppt_string_->publish_state(mppt_string);

  std::string out_src_str = std::to_string(out_src_priority);             // NOLINT
  std::string charger_src_str = std::to_string(charger_src_priority);     // NOLINT
  std::string solar_prio_str = std::to_string(solar_priority);            // NOLINT
  std::string input_v_range_str = std::to_string(input_v_range);          // NOLINT
  std::string machine_type_str = std::to_string(machine_type);            // NOLINT
  std::string max_charging_i_str = std::to_string(max_charging_i);        // NOLINT
  std::string max_ac_charging_i_str = std::to_string(max_ac_charging_i);  // NOLINT
  std::string bat_under_v_str = std::to_string(bat_under_v_raw);          // NOLINT

  if (this->output_source_priority_select_)
    this->output_source_priority_select_->map_and_publish(out_src_str);
  if (this->charger_source_priority_select_)
    this->charger_source_priority_select_->map_and_publish(charger_src_str);
  if (this->solar_power_priority_select_)
    this->solar_power_priority_select_->map_and_publish(solar_prio_str);
  if (this->input_voltage_range_select_)
    this->input_voltage_range_select_->map_and_publish(input_v_range_str);
  if (this->machine_type_select_)
    this->machine_type_select_->map_and_publish(machine_type_str);
  if (this->current_max_charging_current_select_)
    this->current_max_charging_current_select_->map_and_publish(max_charging_i_str);
  if (this->current_max_ac_charging_current_select_)
    this->current_max_ac_charging_current_select_->map_and_publish(max_ac_charging_i_str);
  if (this->battery_under_voltage_select_)
    this->battery_under_voltage_select_->map_and_publish(bat_under_v_str);

  if (this->output_source_priority_switch_)
    this->output_source_priority_switch_->publish_state(out_src_priority != 0);
  if (this->solar_power_priority_switch_)
    this->solar_power_priority_switch_->publish_state(solar_priority == 1);
  if (this->charger_source_priority_solarfirst_switch_)
    this->charger_source_priority_solarfirst_switch_->publish_state(charger_src_priority == 2);
  if (this->charger_source_priority_utility_switch_)
    this->charger_source_priority_utility_switch_->publish_state(charger_src_priority == 0);
  if (this->charger_source_priority_solaronly_switch_)
    this->charger_source_priority_solaronly_switch_->publish_state(charger_src_priority == 3);
}

void Pipsolar::handle_p006mod_(const char *message) {
  if (this->last_p006mod_) {
    this->last_p006mod_->publish_state(message);
  }
  // ^D005<0><mode_char>  — mode digit is at index 6
  if (strlen(message) < 7)
    return;
  const char mode_char = message[6];
  const int mode_id = mode_char - '0';
  if (this->device_mode_id_)
    this->device_mode_id_->publish_state(mode_id);
  if (this->device_mode_) {
    const char *name;
    switch (mode_char) {
      case '1':
        name = "Standby mode";
        break;
      case '2':
        name = "Bypass mode";
        break;
      case '3':
        name = "Battery mode";
        break;
      case '4':
        name = "Fault mode";
        break;
      case '5':
        name = "Hybrid mode";
        break;
      default:
        name = nullptr;
        break;
    }
    if (name)
      this->device_mode_->publish_state(name);
  }
}

void Pipsolar::handle_p007flag_(const char *message) {
  if (this->last_p007flag_)
    this->last_p007flag_->publish_state(message);
  if (strlen(message) < 6)
    return;

  size_t pos = 0;
  skip_start_(message, &pos);

  binary_sensor::BinarySensor *flag_sensors[] = {
      this->silence_buzzer_open_buzzer_,
      this->overload_bypass_function_,
      this->lcd_escape_to_default_,
      this->overload_restart_function_,
      this->over_temperature_restart_function_,
      this->backlight_on_,
      this->alarm_on_when_primary_source_interrupt_,
      this->fault_code_record_,
      this->power_saving_,
  };
  for (auto *sensor : flag_sensors)
    this->publish_binary_sensor_(parse_number<int32_t>(read_field_(message, &pos)).value_or(0) != 0, sensor);
}

void Pipsolar::handle_p005fws_(const char *message) {
  if (this->last_p005fws_)
    this->last_p005fws_->publish_state(message);
  if (strlen(message) < 6)
    return;

  size_t pos = 0;
  skip_start_(message, &pos);

  const int fault_code = parse_number<int32_t>(read_field_(message, &pos)).value_or(0);
  if (this->fault_code_)
    this->fault_code_->publish_state(fault_code);

  binary_sensor::BinarySensor *warn_sensors[] = {
      this->warning_line_fail_,
      this->warning_output_circuit_short_,
      this->warning_over_temperature_,
      this->warning_fan_lock_,
      this->warning_battery_voltage_high_,
      this->warning_battery_low_alarm_,
      this->warning_battery_under_shutdown_,
      this->warning_over_load_,
      this->warning_eeprom_failed_,
      this->warning_power_limit_,
      this->warning_pv1_voltage_high_,
      this->warning_pv2_voltage_high_,
      this->warning_mppt1_overload_,
      this->warning_mppt2_overload_,
      this->warning_scc1_battery_too_low_to_charge_,
      this->warning_scc2_battery_too_low_to_charge_,
  };
  for (auto *sensor : warn_sensors)
    this->publish_binary_sensor_(parse_number<int32_t>(read_field_(message, &pos)).value_or(0) != 0, sensor);
}

void Pipsolar::handle_p005et_(const char *message) {
  if (this->last_p005et_)
    this->last_p005et_->publish_state(message);
  if (strlen(message) < 6)
    return;
  size_t pos = 0;
  skip_start_(message, &pos);
  read_sensor_(message, &pos, this->total_generated_energy_);
}

void Pipsolar::publish_mppt_status_(text_sensor::TextSensor *sensor, int value) {
  if (!sensor)
    return;
  switch (value) {
    case 0:
      sensor->publish_state("abnormal");
      break;
    case 1:
      sensor->publish_state("normal but not charging");
      break;
    case 2:
      sensor->publish_state("charging");
      break;
    default:
      break;
  }
}

void Pipsolar::publish_direction_(text_sensor::TextSensor *sensor, int value) {
  if (!sensor)
    return;
  switch (value) {
    case 0:
      sensor->publish_state("donothing");
      break;
    case 1:
      sensor->publish_state("input");
      break;
    case 2:
      sensor->publish_state("output");
      break;
    default:
      break;
  }
}

void Pipsolar::publish_binary_sensor_(esphome::optional<bool> b, binary_sensor::BinarySensor *sensor) {
  if (sensor) {
    if (b.has_value()) {
      sensor->publish_state(b.value());
    } else {
      sensor->invalidate_state();
    }
  }
}

void Pipsolar::dump_config() {
  ESP_LOGCONFIG(TAG, "PI18 enabled polling commands:");
  for (auto &enabled_polling_command : this->enabled_polling_commands_) {
    if (enabled_polling_command.length != 0) {
      ESP_LOGCONFIG(TAG, "%s", enabled_polling_command.command);
    }
  }
}

void Pipsolar::update() {
  for (auto &enabled_polling_command : this->enabled_polling_commands_) {
    if (enabled_polling_command.length != 0) {
      enabled_polling_command.needs_update = true;
    }
  }
}

void Pipsolar::add_polling_command_(const char *command, ENUMPollingCommand polling_command) {
  for (auto &enabled_polling_command : this->enabled_polling_commands_) {
    if (enabled_polling_command.length == strlen(command)) {
      uint8_t len = strlen(command);
      if (memcmp(enabled_polling_command.command, command, len) == 0) {
        return;
      }
    }
    if (enabled_polling_command.length == 0) {
      size_t length = strlen(command);
      enabled_polling_command.command = new uint8_t[length + 1];  // NOLINT(cppcoreguidelines-owning-memory)
      for (size_t i = 0; i < length + 1; i++) {
        enabled_polling_command.command[i] = (uint8_t) command[i];
      }
      enabled_polling_command.errors = 0;
      enabled_polling_command.identifier = polling_command;
      enabled_polling_command.length = length;
      enabled_polling_command.needs_update = true;
      return;
    }
  }
}

uint16_t Pipsolar::pipsolar_crc_(uint8_t *msg, uint8_t len) {
  uint16_t crc = crc16be(msg, len);
  uint8_t crc_low = crc & 0xff;
  uint8_t crc_high = crc >> 8;
  if (crc_low == 0x28 || crc_low == 0x0d || crc_low == 0x0a)
    crc_low++;
  if (crc_high == 0x28 || crc_high == 0x0d || crc_high == 0x0a)
    crc_high++;
  crc = (crc_high << 8) | crc_low;
  return crc;
}

void Pipsolar::handle_p007pgs0_(const char *message) {
  size_t pos = 0;
  skip_start_(message, &pos);

  for (int i = 0; i < 9; i++)
    skip_field_(message, &pos);

  read_sensor_(message, &pos, this->total_ac_output_apparent_power_);
  read_sensor_(message, &pos, this->total_ac_output_active_power_);
  skip_field_(message, &pos);
  read_sensor_(message, &pos, this->total_output_load_percent_);

  for (int i = 0; i < 3; i++)
    skip_field_(message, &pos);

  read_sensor_(message, &pos, this->total_battery_charging_current_);
}

void Pipsolar::skip_start_(const char *message, size_t *pos) {
  const size_t len = strlen(message);
  *pos = (len >= 5) ? 5 : len;
}

void Pipsolar::skip_field_(const char *message, size_t *pos) {
  while (message[*pos] != '\0' && message[*pos] != ',')
    (*pos)++;
  if (message[*pos] == ',')
    (*pos)++;
}

std::string Pipsolar::read_field_(const char *message, size_t *pos) {
  const size_t begin = *pos;
  while (message[*pos] != '\0' && message[*pos] != ',')
    (*pos)++;
  std::string field(message, begin, *pos - begin);  // NOLINT
  if (message[*pos] == ',')
    (*pos)++;
  return field;
}

void Pipsolar::read_sensor_(const char *message, size_t *pos, sensor::Sensor *sensor, float scale) {
  if (sensor != nullptr) {
    auto val = parse_number<float>(read_field_(message, pos));
    sensor->publish_state(val.has_value() ? val.value() * scale : NAN);
  } else {
    skip_field_(message, pos);
  }
}

}  // namespace esphome::pi18
