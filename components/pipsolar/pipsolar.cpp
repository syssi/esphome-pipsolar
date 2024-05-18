#include "pipsolar.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pipsolar {

static const char *const TAG = "pipsolar";

void Pipsolar::setup() {
  this->state_ = STATE_IDLE;
  this->command_start_millis_ = 0;
}

void Pipsolar::empty_uart_buffer_() {
  uint8_t byte;
  while (this->available()) {
    this->read_byte(&byte);
  }
}

void Pipsolar::loop() {
  // Read message
  if (this->state_ == STATE_IDLE) {
    this->empty_uart_buffer_();
    switch (this->send_next_command_()) {
      case 0:
        // no command send (empty queue) time to poll
        if (millis() - this->last_poll_ > this->update_interval_) {
          this->send_next_poll_();
          this->last_poll_ = millis();
        }
        return;
        break;
      case 1:
        // command send
        return;
        break;
    }
  }
  if (this->state_ == STATE_COMMAND_COMPLETE) {
    if (this->check_incoming_length_(4)) {
      ESP_LOGD(TAG, "response length for command OK");
      if (this->check_incoming_crc_()) {
        // crc ok
        if (this->read_buffer_[1] == 'A' && this->read_buffer_[2] == 'C' && this->read_buffer_[3] == 'K') {
          ESP_LOGD(TAG, "command successful");
        } else {
          ESP_LOGD(TAG, "command not successful");
        }
        this->command_queue_[this->command_queue_position_] = std::string("");
        this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
        this->state_ = STATE_IDLE;

      } else {
        // crc failed
        this->command_queue_[this->command_queue_position_] = std::string("");
        this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
        this->state_ = STATE_IDLE;
      }
    } else {
      ESP_LOGD(TAG, "response length for command %s not OK: with length %zu",
               this->command_queue_[this->command_queue_position_].c_str(), this->read_pos_);
      this->command_queue_[this->command_queue_position_] = std::string("");
      this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
      this->state_ = STATE_IDLE;
    }
  }

  if (this->state_ == STATE_POLL_DECODED) {
    std::string mode;
    switch (this->used_polling_commands_[this->last_polling_command_].identifier) {
      case POLLING_P006MOD:
        if (this->device_mode_) {
          mode = value_device_mode_;
          switch (value_device_mode_) {
            case '0':
              break;
            case '1':
              this->device_mode_->publish_state("Standby mode");
              break;
            case '2':
              this->device_mode_->publish_state("Bypass mode");
              break;
            case '3':
              this->device_mode_->publish_state("Battery mode");
              break;
            case '4':
              this->device_mode_->publish_state("Fault mode");
              break;
            case '5':
              this->device_mode_->publish_state("Hybrid mode");
              break;
            default:
              this->device_mode_->publish_state(mode);
              break;
          }
        }

      case POLLING_P007PIRI:
        if (this->grid_rating_voltage_) {
          this->grid_rating_voltage_->publish_state(value_grid_rating_voltage_ * 0.1);
        }
        if (this->grid_rating_current_) {
          this->grid_rating_current_->publish_state(value_grid_rating_current_ * 0.1);
        }
        if (this->ac_output_rating_voltage_) {
          this->ac_output_rating_voltage_->publish_state(value_ac_output_rating_voltage_ * 0.1);
        }
        if (this->ac_output_rating_frequency_) {
          this->ac_output_rating_frequency_->publish_state(value_ac_output_rating_frequency_ * 0.1);
        }
        if (this->ac_output_rating_current_) {
          this->ac_output_rating_current_->publish_state(value_ac_output_rating_current_ * 0.1);
        }
        if (this->ac_output_rating_apparent_power_) {
          this->ac_output_rating_apparent_power_->publish_state(value_ac_output_rating_apparent_power_);
        }
        if (this->ac_output_rating_active_power_) {
          this->ac_output_rating_active_power_->publish_state(value_ac_output_rating_active_power_);
        }
        if (this->battery_rating_voltage_) {
          this->battery_rating_voltage_->publish_state(value_battery_rating_voltage_ * 0.1);
        }
        if (this->battery_recharge_voltage_) {
          this->battery_recharge_voltage_->publish_state(value_battery_recharge_voltage_ * 0.1);
        }
        if (this->battery_redischarge_voltage_) {
          this->battery_redischarge_voltage_->publish_state(value_battery_redischarge_voltage_ * 0.1);
        }
        if (this->battery_under_voltage_) {
          this->battery_under_voltage_->publish_state(value_battery_under_voltage_ * 0.1);
        }
        if (this->battery_bulk_voltage_) {
          this->battery_bulk_voltage_->publish_state(value_battery_bulk_voltage_ * 0.1);
        }
        if (this->battery_float_voltage_) {
          this->battery_float_voltage_->publish_state(value_battery_float_voltage_ * 0.1);
        }
        if (this->battery_type_) {
          this->battery_type_->publish_state(value_battery_type_);
        }

        //if (this->current_max_ac_charging_current_) {
        //  this->current_max_ac_charging_current_->publish_state(value_current_max_ac_charging_current_);
        //}
        if (this->current_max_ac_charging_current_select_) {
          std::string value = esphome::to_string(value_current_max_ac_charging_current_);
          this->current_max_ac_charging_current_select_->map_and_publish(value);
        }

        //if (this->current_max_charging_current_) {
        //  this->current_max_charging_current_->publish_state(value_current_max_charging_current_);
        //}
        // SELECT OPTION for current_max_charging_current
        if (this->current_max_charging_current_select_) {
          std::string value = esphome::to_string(value_current_max_charging_current_);
          this->current_max_charging_current_select_->map_and_publish(value);
        }

        // SELECT OPTION for output source priority
        if (this->output_source_priority_select_) {
          std::string value = esphome::to_string(value_output_source_priority_);
          this->output_source_priority_select_->map_and_publish(value);
        }
        // SELECT OPTION for charger source priority
        if (this->charger_source_priority_select_) {
          std::string value = esphome::to_string(value_charger_source_priority_);
          this->charger_source_priority_select_->map_and_publish(value);
        }
        // SELECT OPTION for solar power priority
        if (this->solar_power_priority_select_) {
          std::string value = esphome::to_string(value_solar_power_priority_);
          this->solar_power_priority_select_->map_and_publish(value);
        }
        // SELECT OPTION for solar power priority
        if (this->machine_type_select_) {
          std::string value = esphome::to_string(value_machine_type_);
          this->machine_type_select_->map_and_publish(value);
        }
        // SELECT OPTION for input voltage range
        if (this->input_voltage_range_select_) {
          std::string value = esphome::to_string(value_input_voltage_range_);
          this->input_voltage_range_select_->map_and_publish(value);
        }

        if (this->parallel_max_num_) {
          this->parallel_max_num_->publish_state(value_parallel_max_num_);
        }
        if (this->topology_) {
          this->topology_->publish_state(value_topology_);
        }
        if (this->output_mode_) {
          this->output_mode_->publish_state(value_output_mode_);
        }
        if (this->mppt_string_) {
          this->mppt_string_->publish_state(value_mppt_string_);
        }

        this->state_ = STATE_IDLE;
        break;
      case POLLING_P005GS:
        if (this->grid_voltage_) {
          this->grid_voltage_->publish_state(value_grid_voltage_ * 0.1);
        }
        if (this->grid_frequency_) {
          this->grid_frequency_->publish_state(value_grid_frequency_ * 0.1);
        }
        if (this->ac_output_voltage_) {
          this->ac_output_voltage_->publish_state(value_ac_output_voltage_ * 0.1);
        }
        if (this->ac_output_frequency_) {
          this->ac_output_frequency_->publish_state(value_ac_output_frequency_ * 0.1);
        }
        if (this->ac_output_apparent_power_) {
          this->ac_output_apparent_power_->publish_state(value_ac_output_apparent_power_);
        }
        if (this->ac_output_active_power_) {
          this->ac_output_active_power_->publish_state(value_ac_output_active_power_);
        }
        if (this->output_load_percent_) {
          this->output_load_percent_->publish_state(value_output_load_percent_);
        }

        if (this->battery_voltage_) {
          this->battery_voltage_->publish_state(value_battery_voltage_ * 0.1);
        }
        if (this->battery_voltage_scc_) {
          this->battery_voltage_scc_->publish_state(value_battery_voltage_scc_ * 0.1);
        }
        if (this->battery_voltage_scc2_) {
          this->battery_voltage_scc2_->publish_state(value_battery_voltage_scc2_ * 0.1);
        }
        if (this->battery_discharge_current_) {
          this->battery_discharge_current_->publish_state(value_battery_discharge_current_ * -1);
        }
        if (this->battery_charging_current_) {
          this->battery_charging_current_->publish_state(value_battery_charging_current_);
        }
        if (this->battery_capacity_percent_) {
          this->battery_capacity_percent_->publish_state(value_battery_capacity_percent_);
        }
        if (this->inverter_heat_sink_temperature_) {
          this->inverter_heat_sink_temperature_->publish_state(value_inverter_heat_sink_temperature_);
        }
        if (this->mppt1_charger_temperature_) {
          this->mppt1_charger_temperature_->publish_state(value_mppt1_charger_temperature_);
        }
        if (this->mppt2_charger_temperature_) {
          this->mppt2_charger_temperature_->publish_state(value_mppt2_charger_temperature_);
        }
        if (this->pv1_input_power_) {
          this->pv1_input_power_->publish_state(value_pv1_input_power_);
        }
        if (this->pv2_input_power_) {
          this->pv2_input_power_->publish_state(value_pv2_input_power_);
        }
        if (this->pv1_input_voltage_) {
          this->pv1_input_voltage_->publish_state(value_pv1_input_voltage_ * 0.1);
        }
        if (this->pv2_input_voltage_) {
          this->pv2_input_voltage_->publish_state(value_pv2_input_voltage_ * 0.1);
        }
        if (this->setting_value_configuration_state_) {
          this->setting_value_configuration_state_->publish_state(value_setting_value_configuration_state_);
        }
        // special for mppt1_charger_status Text
        if (this->mppt1_charger_status_) {
          mode = value_mppt1_charger_status_;
          switch (value_mppt1_charger_status_) {
            case '0':
              this->mppt1_charger_status_->publish_state("abnormal");
              break;
            case '1':
              this->mppt1_charger_status_->publish_state("normal but not charged");
              break;
            case '2':
              this->mppt1_charger_status_->publish_state("charging");
              break;
          }
        }
        // special for mppt2_charger_status Text
        if (this->mppt2_charger_status_) {
          mode = value_mppt2_charger_status_;
          switch (value_mppt2_charger_status_) {
            case '0':
              this->mppt2_charger_status_->publish_state("abnormal");
              break;
            case '1':
              this->mppt2_charger_status_->publish_state("normal but not charged");
              break;
            case '2':
              this->mppt2_charger_status_->publish_state("charging");
              break;
          }
        }

        // special for load connection Text
        if (this->load_connection_) {
          mode = value_load_connection_;
          switch (value_load_connection_) {
            case '0':
              this->load_connection_->publish_state("disconnect");
              break;
            case '1':
              this->load_connection_->publish_state("connect");
              break;
          }
        }
        // special for battery_power_direction Text
        if (this->battery_power_direction_) {
          mode = value_battery_power_direction_;
          switch (value_battery_power_direction_) {
            case '0':
              this->battery_power_direction_->publish_state("donothing");
              break;
            case '1':
              this->battery_power_direction_->publish_state("charge");
              break;
            case '2':
              this->battery_power_direction_->publish_state("discharge");
              break;
          }
        }

        if (this->dc_ac_power_direction_) {
          this->dc_ac_power_direction_->publish_state(value_dc_ac_power_direction_);
        }
        if (this->line_power_direction_) {
          this->line_power_direction_->publish_state(value_line_power_direction_);
        }
        if (this->local_parallel_id_) {
          this->local_parallel_id_->publish_state(value_local_parallel_id_);
        }
        this->state_ = STATE_IDLE;
        break;

      case POLLING_P007FLAG:
        if (this->silence_buzzer_open_buzzer_) {
          this->silence_buzzer_open_buzzer_->publish_state(value_silence_buzzer_open_buzzer_);
        }
        if (this->overload_bypass_function_) {
          this->overload_bypass_function_->publish_state(value_overload_bypass_function_);
        }
        if (this->lcd_escape_to_default_) {
          this->lcd_escape_to_default_->publish_state(value_lcd_escape_to_default_);
        }
        if (this->overload_restart_function_) {
          this->overload_restart_function_->publish_state(value_overload_restart_function_);
        }
        if (this->over_temperature_restart_function_) {
          this->over_temperature_restart_function_->publish_state(value_over_temperature_restart_function_);
        }
        if (this->backlight_on_) {
          this->backlight_on_->publish_state(value_backlight_on_);
        }
        if (this->alarm_on_when_primary_source_interrupt_) {
          this->alarm_on_when_primary_source_interrupt_->publish_state(value_alarm_on_when_primary_source_interrupt_);
        }
        if (this->fault_code_record_) {
          this->fault_code_record_->publish_state(value_fault_code_record_);
        }

        // SWICHES
        if (this->silence_buzzer_open_buzzer_switch_) {
          this->silence_buzzer_open_buzzer_switch_->publish_state(value_silence_buzzer_open_buzzer_ == 1);
        }
        if (this->overload_bypass_function_switch_) {
          this->overload_bypass_function_switch_->publish_state(value_overload_bypass_function_ == 1);
        }
        if (this->lcd_escape_to_default_switch_) {
          this->lcd_escape_to_default_switch_->publish_state(value_lcd_escape_to_default_ == 1);
        }
        if (this->overload_restart_function_switch_) {
          this->overload_restart_function_switch_->publish_state(value_overload_restart_function_ == 1);
        }
        if (this->over_temperature_restart_function_switch_) {
          this->over_temperature_restart_function_switch_->publish_state(value_over_temperature_restart_function_ == 1);
        }
        if (this->backlight_on_switch_) {
          this->backlight_on_switch_->publish_state(value_backlight_on_ == 1);
        }
        if (this->alarm_on_when_primary_source_interrupt_switch_) {
          this->alarm_on_when_primary_source_interrupt_switch_->publish_state(
              value_alarm_on_when_primary_source_interrupt_ == 1);
        }
        if (this->fault_code_record_switch_) {
          this->fault_code_record_switch_->publish_state(value_fault_code_record_ == 1);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_P005FWS:
        if (this->fault_code_) {
          this->fault_code_->publish_state(value_fault_code_);
        }
        if (this->warning_line_fail_) {
          this->warning_line_fail_->publish_state(value_warning_line_fail_);
        }
        if (this->warning_output_circuit_short_) {
          this->warning_output_circuit_short_->publish_state(value_warning_output_circuit_short_);
        }
        if (this->warning_over_temperature_) {
          this->warning_over_temperature_->publish_state(value_warning_over_temperature_);
        }
        if (this->warning_fan_lock_) {
          this->warning_fan_lock_->publish_state(value_warning_fan_lock_);
        }
        if (this->warning_battery_voltage_high_) {
          this->warning_battery_voltage_high_->publish_state(value_warning_battery_voltage_high_);
        }
        if (this->warning_battery_low_alarm_) {
          this->warning_battery_low_alarm_->publish_state(value_warning_battery_low_alarm_);
        }
        if (this->warning_battery_under_shutdown_) {
          this->warning_battery_under_shutdown_->publish_state(value_warning_battery_under_shutdown_);
        }
        if (this->warning_over_load_) {
          this->warning_over_load_->publish_state(value_warning_over_load_);
        }
        if (this->warning_eeprom_failed_) {
          this->warning_eeprom_failed_->publish_state(value_warning_eeprom_failed_);
        }
        if (this->warning_power_limit_) {
          this->warning_power_limit_->publish_state(value_warning_power_limit_);
        }
        if (this->warning_pv1_voltage_high_) {
          this->warning_pv1_voltage_high_->publish_state(value_warning_pv1_voltage_high_);
        }
        if (this->warning_pv2_voltage_high_) {
          this->warning_pv2_voltage_high_->publish_state(value_warning_pv2_voltage_high_);
        }
        if (this->warning_mppt1_overload_) {
          this->warning_mppt1_overload_->publish_state(value_warning_mppt1_overload_);
        }
        if (this->warning_mppt2_overload_) {
          this->warning_mppt2_overload_->publish_state(value_warning_mppt2_overload_);
        }
        if (this->scc1_battery_too_low_to_charge_) {
          this->scc1_battery_too_low_to_charge_->publish_state(value_scc1_battery_too_low_to_charge_);
        }
        if (this->scc2_battery_too_low_to_charge_) {
          this->scc2_battery_too_low_to_charge_->publish_state(value_scc2_battery_too_low_to_charge_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_P005ET:
        if (this->total_generated_energy_) {
          this->total_generated_energy_->publish_state(value_total_generated_energy_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_P007PGS0:
        if (this->total_ac_output_apparent_power_) {
          this->total_ac_output_apparent_power_->publish_state(value_total_ac_output_apparent_power_);
        }
        if (this->total_ac_output_active_power_) {
          this->total_ac_output_active_power_->publish_state(value_total_ac_output_active_power_);
        }
        if (this->total_output_load_percent_) {
          this->total_output_load_percent_->publish_state(value_total_output_load_percent_);
        }
        if (this->total_battery_charging_current_) {
          this->total_battery_charging_current_->publish_state(value_total_battery_charging_current_);
        }
        this->state_ = STATE_IDLE;
        break;
    }
  }

  if (this->state_ == STATE_POLL_CHECKED) {
    bool enabled = true;
    std::string fc;
    int ind;
    char tmp[PIPSOLAR_READ_BUFFER_LENGTH];
    sprintf(tmp, "%s", this->read_buffer_);
    switch (this->used_polling_commands_[this->last_polling_command_].identifier) {
      case POLLING_P007PIRI:
        ESP_LOGD(TAG, "Decode P007PIRI");
        sscanf(  //"^D0892300,243,2300,500,243,5600,5600,480,470,530,440,554,544,2,040,090,1,0,1,9,0,0,1,0,1,00\xD9\xA1\r"
            tmp,  // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
            "^D%3d%f,%f,%f,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &ind,
            &value_grid_rating_voltage_, &value_grid_rating_current_, &value_ac_output_rating_voltage_,
            &value_ac_output_rating_frequency_, &value_ac_output_rating_current_,
            &value_ac_output_rating_apparent_power_, &value_ac_output_rating_active_power_,
            &value_battery_rating_voltage_, &value_battery_recharge_voltage_, &value_battery_redischarge_voltage_,
            &value_battery_under_voltage_, &value_battery_bulk_voltage_, &value_battery_float_voltage_,
            &value_battery_type_, &value_current_max_ac_charging_current_, &value_current_max_charging_current_,
            &value_input_voltage_range_, &value_output_source_priority_, &value_charger_source_priority_,
            &value_parallel_max_num_, &value_machine_type_, &value_topology_, &value_output_mode_,
            &value_solar_power_priority_, &value_mppt_string_);
        this->state_ = STATE_POLL_DECODED;
        break;

      case POLLING_P005GS:
        ESP_LOGD(TAG, "Decode P005GS");
        //        "^D1062135,499,2135,499,2102,2102,037,544,000,000,000,039,095,049,000,000,0000,0000,0000,0000,0,0,0,1,1,1,1,1\e\'\r"
        sscanf(tmp, "^D%3d%f,%f,%f,%f,%d,%d,%d,%f,%f,%f,%d,%d,%d,%d,%f, %f,%f,%f,%f,%f,%d,%c,%c,%c,%c,%d,%d,%d", &ind,
               &value_grid_voltage_, &value_grid_frequency_, &value_ac_output_voltage_, &value_ac_output_frequency_,
               &value_ac_output_apparent_power_, &value_ac_output_active_power_, &value_output_load_percent_,
               &value_battery_voltage_, &value_battery_voltage_scc_, &value_battery_voltage_scc2_,
               &value_battery_discharge_current_, &value_battery_charging_current_, &value_battery_capacity_percent_,
               &value_inverter_heat_sink_temperature_, &value_mppt1_charger_temperature_,
               &value_mppt2_charger_temperature_, &value_pv1_input_power_, &value_pv2_input_power_,
               &value_pv1_input_voltage_, &value_pv2_input_voltage_, &value_setting_value_configuration_state_,
               &value_mppt1_charger_status_, &value_mppt2_charger_status_, &value_load_connection_,
               &value_battery_power_direction_, &value_dc_ac_power_direction_, &value_line_power_direction_,
               &value_local_parallel_id_);
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_P006MOD:
        ESP_LOGD(TAG, "Decode P006MOD");
        this->value_device_mode_ = char(this->read_buffer_[6]);
        //                        if (this->last_qmod_) {
        //                            this->last_qmod_->publish_state(tmp);
        //                        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_P007FLAG:
        ESP_LOGD(TAG, "Decode P007FLAG");
        // result like:"^D0201,1,1,0,0,1,0,1,0\xF6=\r"
        // get through all char: ignore first "(" Enable flag on 'E', Disable on 'D') else set the corresponding value
        sscanf(tmp,  // 1  2  3  4  5  6  7  8  9
               "^D%3d%d,%d,%d,%d,%d,%d,%d,%d", &ind, &value_silence_buzzer_open_buzzer_,
               &value_overload_bypass_function_, &value_lcd_escape_to_default_, &value_overload_restart_function_,
               &value_over_temperature_restart_function_, &value_backlight_on_,
               &value_alarm_on_when_primary_source_interrupt_, &value_fault_code_record_);
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_P005FWS:
        // result like:"^D03900,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0\xA6|\r"
        ESP_LOGD(TAG, "Decode P005FWS");
        fc = tmp[5];
        fc += tmp[6];
        this->value_fault_code_ = parse_number<int>(fc).value_or(0);
        for (size_t i = 8; i < strlen(tmp); i = i + 2) {
          enabled = tmp[i] == '1';
          //                            ESP_LOGD(TAG, "tmp %s", tmp[i]);
          switch (i) {
            case 8:
              this->value_warning_line_fail_ = enabled;
              break;
            case 10:
              this->value_warning_output_circuit_short_ = enabled;
              break;
            case 12:
              this->value_warning_over_temperature_ = enabled;
              break;
            case 14:
              this->value_warning_fan_lock_ = enabled;
              break;
            case 16:
              this->value_warning_battery_voltage_high_ = enabled;
              break;
            case 18:
              this->value_warning_battery_low_alarm_ = enabled;
              break;
            case 20:
              this->value_warning_battery_under_shutdown_ = enabled;
              break;
            case 22:
              this->value_warning_over_load_ = enabled;
              break;
            case 24:
              this->value_warning_eeprom_failed_ = enabled;
              break;
            case 26:
              this->value_warning_power_limit_ = enabled;
              break;
            case 28:
              this->value_warning_pv1_voltage_high_ = enabled;
              break;
            case 30:
              this->value_warning_pv2_voltage_high_ = enabled;
              break;
            case 32:
              this->value_warning_mppt1_overload_ = enabled;
              break;
            case 34:
              this->value_warning_mppt2_overload_ = enabled;
              break;
            case 36:
              this->value_scc1_battery_too_low_to_charge_ = enabled;
              break;
            case 38:
              this->value_scc2_battery_too_low_to_charge_ = enabled;
              break;
          }
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_P005ET:
        ESP_LOGD(TAG, "Decode P005ET");
        sscanf(tmp, "^D%3d%08d", &ind, &value_total_generated_energy_);
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_P007PGS0:
        ESP_LOGD(TAG, "Decode P007PGS0");
        //"^D1131,3,00,2384,500,2301,500,1287,1286,02621,02600,022,023,522,025,000,000,080,0027,0000,1200,0000,2,0,1,2,2,0,048D\xDF\r"
        sscanf(tmp,  // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
               "^D%3d%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &ind,
               &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind,
               //                                &value_one_parallel_id_connection_status_, //1 A
               //                                &value_one_work_mode_, //2 B
               //                                &value_one_faults_code_, //3 CC
               //                                &value_one_grid_voltage_, //4 DDDD
               //                                &value_one_grid_frequency_, //5 EEE
               //                                &value_one_ac_output_voltage_, // 6 FFFF
               //                                &value_one_ac_output_frequency_, // 7 GGG
               //                                &value_one_ac_output_apparent_power_, //8 HHHH
               //                                &value_one_ac_output_active_power_, //9 IIII
               &value_total_ac_output_apparent_power_,  // 10 JJJJJ
               &value_total_ac_output_active_power_,    // 11 KKKKK
               &ind,                                    // &value_one_output_load_percent_, //12 LLL
               &value_total_output_load_percent_,       // 13 MMM
               &ind,                                    // &value_one_battery_voltage_, //14 NNN
               &ind,                                    // &value_one_battery_discharge_current_, //15 OOO
               &ind,                                    // &value_one_battery_charging_current_, //16 PPP
               &value_total_battery_charging_current_,  // 17 QQQ
               &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind, &ind
               //                                &value_one_battery_capacity_, //18 MMM
               //                                &value_one_pv1_input_power_, //19 RRRR
               //                                &value_one_pv2_input_power_, //20 SSSS
               //                                &value_one_pv1_input_voltage_, //21 TTTT
               //                                &value_one_pv2_input_voltage_, // 22 UUUU
               //                                &value_one_mppt1_charger_status_, //23 V
               //                                &value_one_mppt2_charger_status_, // 24 W
               //                                &value_one_load_connection_, //25 X
               //                                &value_one_battery_power_direction_, //26 Y
               //                                &value_one_dc_ac_power_direction_, // 27 Z
               //                                &value_one_line_power_direction_, // 28 a
               //                                &value_one_max_temperature_ //29 bbb
        );
        this->state_ = STATE_POLL_DECODED;
        break;
      default:
        this->state_ = STATE_IDLE;
        break;
    }
    return;
  }

  if (this->state_ == STATE_POLL_COMPLETE) {
    if (this->check_incoming_crc_()) {
      if (this->read_buffer_[0] == '(' && this->read_buffer_[1] == 'N' && this->read_buffer_[2] == 'A' &&
          this->read_buffer_[3] == 'K') {
        this->state_ = STATE_IDLE;
        return;
      }
      // crc ok
      this->state_ = STATE_POLL_CHECKED;
      return;
    } else {
      this->state_ = STATE_IDLE;
    }
  }

  if (this->state_ == STATE_COMMAND || this->state_ == STATE_POLL) {
    while (this->available()) {
      uint8_t byte;
      this->read_byte(&byte);

      if (this->read_pos_ == PIPSOLAR_READ_BUFFER_LENGTH) {
        this->read_pos_ = 0;
        this->empty_uart_buffer_();
      }
      this->read_buffer_[this->read_pos_] = byte;
      this->read_pos_++;

      // end of answer
      if (byte == 0x0D) {
        this->read_buffer_[this->read_pos_] = 0;
        this->empty_uart_buffer_();
        if (this->state_ == STATE_POLL) {
          this->state_ = STATE_POLL_COMPLETE;
        }
        if (this->state_ == STATE_COMMAND) {
          this->state_ = STATE_COMMAND_COMPLETE;
        }
      }
    }  // available
  }
  if (this->state_ == STATE_COMMAND) {
    if (millis() - this->command_start_millis_ > esphome::pipsolar::Pipsolar::COMMAND_TIMEOUT) {
      // command timeout
      const char *command = this->command_queue_[this->command_queue_position_].c_str();
      this->command_start_millis_ = millis();
      ESP_LOGD(TAG, "timeout command from queue: %s", command);
      this->command_queue_[this->command_queue_position_] = std::string("");
      this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
      this->state_ = STATE_IDLE;
      return;
    } else {
    }
  }
  if (this->state_ == STATE_POLL) {
    if (millis() - this->command_start_millis_ > esphome::pipsolar::Pipsolar::COMMAND_TIMEOUT) {
      // command timeout
      ESP_LOGD(TAG, "timeout command to poll: %s", this->used_polling_commands_[this->last_polling_command_].command);
      this->state_ = STATE_IDLE;
    } else {
    }
  }
}

uint8_t Pipsolar::check_incoming_length_(uint8_t length) {
  if (this->read_pos_ - 3 == length) {
    return 1;
  }
  return 0;
}

uint8_t Pipsolar::check_incoming_crc_() {
  uint16_t crc16;
  crc16 = cal_crc_half_(read_buffer_, read_pos_ - 3);
  ESP_LOGD(TAG, "checking crc on incoming message");
  if (((uint8_t)((crc16) >> 8)) == read_buffer_[read_pos_ - 3] &&
      ((uint8_t)((crc16) &0xff)) == read_buffer_[read_pos_ - 2]) {
    ESP_LOGD(TAG, "CRC OK");
    read_buffer_[read_pos_ - 1] = 0;
    read_buffer_[read_pos_ - 2] = 0;
    read_buffer_[read_pos_ - 3] = 0;
    return 1;
  }
  ESP_LOGD(TAG, "CRC NOK expected: %X %X but got: %X %X", ((uint8_t)((crc16) >> 8)), ((uint8_t)((crc16) &0xff)),
           read_buffer_[read_pos_ - 3], read_buffer_[read_pos_ - 2]);
  return 0;
}

// send next command used

uint8_t Pipsolar::send_next_command_() {
  uint16_t crc16;
  if (this->command_queue_[this->command_queue_position_].length() != 0) {
    const char *command = this->command_queue_[this->command_queue_position_].c_str();
    uint8_t byte_command[16];
    uint8_t length = this->command_queue_[this->command_queue_position_].length();
    for (uint8_t i = 0; i < length; i++) {
      byte_command[i] = (uint8_t) this->command_queue_[this->command_queue_position_].at(i);
    }
    this->state_ = STATE_COMMAND;
    this->command_start_millis_ = millis();
    this->empty_uart_buffer_();
    this->read_pos_ = 0;
    crc16 = cal_crc_half_(byte_command, length);
    this->write_str(command);
    // checksum
    this->write(((uint8_t)((crc16) >> 8)));   // highbyte
    this->write(((uint8_t)((crc16) &0xff)));  // lowbyte
    // end Byte
    this->write(0x0D);
    ESP_LOGD(TAG, "Sending command from queue: %s with length %d", command, length);
    return 1;
  }
  return 0;
}

void Pipsolar::send_next_poll_() {
  uint16_t crc16;
  this->last_polling_command_ = (this->last_polling_command_ + 1) % 15;
  if (this->used_polling_commands_[this->last_polling_command_].length == 0) {
    this->last_polling_command_ = 0;
  }
  if (this->used_polling_commands_[this->last_polling_command_].length == 0) {
    // no command specified
    return;
  }
  this->state_ = STATE_POLL;
  this->command_start_millis_ = millis();
  this->empty_uart_buffer_();
  this->read_pos_ = 0;
  crc16 = cal_crc_half_(this->used_polling_commands_[this->last_polling_command_].command,
                        this->used_polling_commands_[this->last_polling_command_].length);
  this->write_array(this->used_polling_commands_[this->last_polling_command_].command,
                    this->used_polling_commands_[this->last_polling_command_].length);
  // checksum
  this->write(((uint8_t)((crc16) >> 8)));   // highbyte
  this->write(((uint8_t)((crc16) &0xff)));  // lowbyte
  // end Byte
  this->write(0x0D);
  ESP_LOGD(TAG, "Sending polling command : %s with length %d",
           this->used_polling_commands_[this->last_polling_command_].command,
           this->used_polling_commands_[this->last_polling_command_].length);
}

void Pipsolar::queue_command_(const char *command, uint8_t length) {
  uint8_t next_position = command_queue_position_;
  for (uint8_t i = 0; i < COMMAND_QUEUE_LENGTH; i++) {
    uint8_t testposition = (next_position + i) % COMMAND_QUEUE_LENGTH;
    if (command_queue_[testposition].length() == 0) {
      command_queue_[testposition] = command;
      ESP_LOGD(TAG, "Command queued successfully: %s with length %u at position %d", command,
               command_queue_[testposition].length(), testposition);
      return;
    }
  }
  ESP_LOGD(TAG, "Command queue full dropping command: %s", command);
}

void Pipsolar::switch_command(const std::string &command) {
  ESP_LOGD(TAG, "got command: %s", command.c_str());
  queue_command_(command.c_str(), command.length());
}

void Pipsolar::dump_config() {
  ESP_LOGCONFIG(TAG, "Pipsolar:");
  ESP_LOGCONFIG(TAG, "used commands:");
  for (auto &used_polling_command : this->used_polling_commands_) {
    if (used_polling_command.length != 0) {
      ESP_LOGCONFIG(TAG, "%s", used_polling_command.command);
    }
  }
}

void Pipsolar::update() {}

void Pipsolar::add_polling_command_(const char *command, ENUMPollingCommand polling_command) {
  for (auto &used_polling_command : this->used_polling_commands_) {
    if (used_polling_command.length == strlen(command)) {
      uint8_t len = strlen(command);
      if (memcmp(used_polling_command.command, command, len) == 0) {
        return;
      }
    }
    if (used_polling_command.length == 0) {
      size_t length = strlen(command) + 1;
      const char *beg = command;
      const char *end = command + length;
      used_polling_command.command = new uint8_t[length];  // NOLINT(cppcoreguidelines-owning-memory)
      size_t i = 0;
      for (; beg != end; ++beg, ++i) {
        used_polling_command.command[i] = (uint8_t)(*beg);
      }
      used_polling_command.errors = 0;
      used_polling_command.identifier = polling_command;
      used_polling_command.length = length - 1;
      return;
    }
  }
}

uint16_t Pipsolar::cal_crc_half_(uint8_t *msg, uint8_t len) {
  uint16_t crc;

  uint8_t da;
  uint8_t *ptr;
  uint8_t b_crc_hign;
  uint8_t b_crc_low;

  uint16_t crc_ta[16] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
                         0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};

  ptr = msg;
  crc = 0;

  while (len-- != 0) {
    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= crc_ta[da ^ (*ptr >> 4)];
    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= crc_ta[da ^ (*ptr & 0x0f)];
    ptr++;
  }

  b_crc_low = crc;
  b_crc_hign = (uint8_t)(crc >> 8);

  if (b_crc_low == 0x28 || b_crc_low == 0x0d || b_crc_low == 0x0a)
    b_crc_low++;
  if (b_crc_hign == 0x28 || b_crc_hign == 0x0d || b_crc_hign == 0x0a)
    b_crc_hign++;

  crc = ((uint16_t) b_crc_hign) << 8;
  crc += b_crc_low;
  return (crc);
}

}  // namespace pipsolar
}  // namespace esphome
