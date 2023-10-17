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
          ESP_LOGD(TAG, "command successful: %s", this->command_queue_[this->command_queue_position_].c_str());
        } else {
          ESP_LOGD(TAG, "command not successful: %s", this->command_queue_[this->command_queue_position_].c_str());
        }
        this->command_queue_[this->command_queue_position_] = std::string("");
        this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
        this->state_ = STATE_IDLE;

      } else {
        // crc failed
        ESP_LOGD(TAG, "crc failed: %s", this->command_queue_[this->command_queue_position_].c_str());
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
      case POLLING_QPIRI:
        if (this->grid_rating_voltage_) {
          this->grid_rating_voltage_->publish_state(value_grid_rating_voltage_);
        }
        if (this->grid_rating_current_) {
          this->grid_rating_current_->publish_state(value_grid_rating_current_);
        }
        if (this->ac_output_rating_voltage_) {
          this->ac_output_rating_voltage_->publish_state(value_ac_output_rating_voltage_);
        }
        if (this->ac_output_rating_frequency_) {
          this->ac_output_rating_frequency_->publish_state(value_ac_output_rating_frequency_);
        }
        if (this->ac_output_rating_current_) {
          this->ac_output_rating_current_->publish_state(value_ac_output_rating_current_);
        }
        if (this->ac_output_rating_apparent_power_) {
          this->ac_output_rating_apparent_power_->publish_state(value_ac_output_rating_apparent_power_);
        }
        if (this->ac_output_rating_active_power_) {
          this->ac_output_rating_active_power_->publish_state(value_ac_output_rating_active_power_);
        }
        if (this->battery_rating_voltage_) {
          this->battery_rating_voltage_->publish_state(value_battery_rating_voltage_);
        }
        if (this->battery_recharge_voltage_) {
          this->battery_recharge_voltage_->publish_state(value_battery_recharge_voltage_);
        }
        if (this->battery_recharge_voltage_select_) {
          std::string value = str_snprintf("%.1f", 32, value_battery_recharge_voltage_);
          this->battery_recharge_voltage_select_->map_and_publish(value);
        }
        if (this->battery_under_voltage_) {
          this->battery_under_voltage_->publish_state(value_battery_under_voltage_);
        }
        if (this->battery_cutoff_voltage_select_) {
          std::string value = str_snprintf("%.1f", 32, value_battery_under_voltage_);
          this->battery_cutoff_voltage_select_->map_and_publish(value);
        }
        if (this->battery_bulk_voltage_) {
          this->battery_bulk_voltage_->publish_state(value_battery_bulk_voltage_);
        }
        if (this->battery_bulk_voltage_select_) {
          std::string value = str_snprintf("%.1f", 32, value_battery_bulk_voltage_);
          this->battery_bulk_voltage_select_->map_and_publish(value);
        }
        if (this->battery_float_voltage_) {
          this->battery_float_voltage_->publish_state(value_battery_float_voltage_);
        }
        if (this->battery_float_voltage_select_) {
          std::string value = str_snprintf("%.1f", 32, value_battery_float_voltage_);
          this->battery_float_voltage_select_->map_and_publish(value);
        }
        if (this->battery_type_) {
          this->battery_type_->publish_state(value_battery_type_);
        }
        if (this->battery_type_select_) {
          std::string value = esphome::to_string(value_battery_type_);
          this->battery_type_select_->map_and_publish(value);
        }
        if (this->current_max_ac_charging_current_) {
          this->current_max_ac_charging_current_->publish_state(value_current_max_ac_charging_current_);
        }
        //  select for current_max_ac_charging_current
        if (this->current_max_ac_charging_current_select_) {
          std::string value = esphome::to_string(value_current_max_ac_charging_current_);
          this->current_max_ac_charging_current_select_->map_and_publish(value);
        }
        
        if (this->current_max_charging_current_) {
          this->current_max_charging_current_->publish_state(value_current_max_charging_current_);
        }
         //select for current_max_charging_current
        if (this->current_max_charging_current_select_) {
          std::string value = esphome::to_string(value_current_max_charging_current_);
          this->current_max_charging_current_select_->map_and_publish(value);
        }

        if (this->input_voltage_range_) {
          this->input_voltage_range_->publish_state(value_input_voltage_range_);
        }
        // special for input voltage range switch
        if (this->input_voltage_range_switch_) {
          this->input_voltage_range_switch_->publish_state(value_input_voltage_range_ == 1);
        }
        if (this->output_source_priority_) {
          this->output_source_priority_->publish_state(value_output_source_priority_);
        }
        // special for output source priority select
        if (this->output_source_priority_select_) {
          std::string value = esphome::to_string(value_output_source_priority_);
          this->output_source_priority_select_->map_and_publish(value);
        }
        // special for output source priority switches
        if (this->output_source_priority_utility_switch_) {
          this->output_source_priority_utility_switch_->publish_state(value_output_source_priority_ == 0);
        }
        if (this->output_source_priority_solar_switch_) {
          this->output_source_priority_solar_switch_->publish_state(value_output_source_priority_ == 1);
        }
        if (this->output_source_priority_battery_switch_) {
          this->output_source_priority_battery_switch_->publish_state(value_output_source_priority_ == 2);
        }
        if (this->charger_source_priority_) {
          this->charger_source_priority_->publish_state(value_charger_source_priority_);
        }
        // special for charger source priority select
        if (this->charger_source_priority_select_) {
          std::string value = esphome::to_string(value_charger_source_priority_);
          this->charger_source_priority_select_->map_and_publish(value);
        }
        
        if (this->parallel_max_num_) {
          this->parallel_max_num_->publish_state(value_parallel_max_num_);
        }
        if (this->machine_type_) {
          this->machine_type_->publish_state(value_machine_type_);
        }
        if (this->topology_) {
          this->topology_->publish_state(value_topology_);
        }
        if (this->output_mode_) {
          this->output_mode_->publish_state(value_output_mode_);
        }
        if (this->battery_redischarge_voltage_) {
          this->battery_redischarge_voltage_->publish_state(value_battery_redischarge_voltage_);
        }
        if (this->battery_redischarge_voltage_select_) {
          std::string value = str_snprintf("%.1f", 32, value_battery_redischarge_voltage_);
          this->battery_redischarge_voltage_select_->map_and_publish(value);
        }
        if (this->pv_ok_condition_for_parallel_) {
          this->pv_ok_condition_for_parallel_->publish_state(value_pv_ok_condition_for_parallel_);
        }
        // special for pv ok condition switch
        if (this->pv_ok_condition_for_parallel_switch_) {
          this->pv_ok_condition_for_parallel_switch_->publish_state(value_pv_ok_condition_for_parallel_ == 1);
        }
        if (this->pv_power_balance_) {
          this->pv_power_balance_->publish_state(value_pv_power_balance_ == 1);
        }
        // special for power balance switch
        if (this->pv_power_balance_switch_) {
          this->pv_power_balance_switch_->publish_state(value_pv_power_balance_ == 1);
        }
        if (this->max_charging_time_at_cv_stage_) {
          this->max_charging_time_at_cv_stage_->publish_state(value_max_charging_time_at_cv_stage_);
        }
        if (this->operation_logic_) {
          this->operation_logic_->publish_state(value_operation_logic_);
        }
        if (this->max_discharging_current_) {
          this->max_discharging_current_->publish_state(value_max_discharging_current_);
        }
        if (this->max_discharging_current_select_) {
          std::string value = esphome::to_string(value_max_discharging_current_);
          this->max_discharging_current_select_->map_and_publish(value);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QPIGS:
        if (this->grid_voltage_) {
          this->grid_voltage_->publish_state(value_grid_voltage_);
        }
        if (this->grid_frequency_) {
          this->grid_frequency_->publish_state(value_grid_frequency_);
        }
        if (this->ac_output_voltage_) {
          this->ac_output_voltage_->publish_state(value_ac_output_voltage_);
        }
        if (this->ac_output_frequency_) {
          this->ac_output_frequency_->publish_state(value_ac_output_frequency_);
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
        if (this->bus_voltage_) {
          this->bus_voltage_->publish_state(value_bus_voltage_);
        }
        if (this->battery_voltage_) {
          this->battery_voltage_->publish_state(value_battery_voltage_);
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
        if (this->pv1_input_current_) {
          this->pv1_input_current_->publish_state(value_pv1_input_current_);
        }
        if (this->pv1_input_voltage_) {
          this->pv1_input_voltage_->publish_state(value_pv1_input_voltage_);
        }
        if (this->battery_voltage_scc_) {
          this->battery_voltage_scc_->publish_state(value_battery_voltage_scc_);
        }
        if (this->battery_discharge_current_) {
          this->battery_discharge_current_->publish_state(value_battery_discharge_current_);
        }
        if (this->add_sbu_priority_version_) {
          this->add_sbu_priority_version_->publish_state(value_add_sbu_priority_version_);
        }
        if (this->configuration_status_) {
          this->configuration_status_->publish_state(value_configuration_status_);
        }
        if (this->scc_firmware_version_) {
          this->scc_firmware_version_->publish_state(value_scc_firmware_version_);
        }
        if (this->load_status_) {
          this->load_status_->publish_state(value_load_status_);
        }
        if (this->battery_voltage_to_steady_while_charging_) {
          this->battery_voltage_to_steady_while_charging_->publish_state(
              value_battery_voltage_to_steady_while_charging_);
        }
        if (this->charging_status_) {
          this->charging_status_->publish_state(value_charging_status_);
        }
        if (this->scc_charging_status_) {
          this->scc_charging_status_->publish_state(value_scc_charging_status_);
        }
        if (this->ac_charging_status_) {
          this->ac_charging_status_->publish_state(value_ac_charging_status_);
        }
        if (this->battery_voltage_offset_for_fans_on_) {
          this->battery_voltage_offset_for_fans_on_->publish_state(value_battery_voltage_offset_for_fans_on_ / 10.0f);
        }  //.1 scale
        if (this->eeprom_version_) {
          this->eeprom_version_->publish_state(value_eeprom_version_);
        }
        if (this->pv1_charging_power_) {
          this->pv1_charging_power_->publish_state(value_pv1_charging_power_);
        }
        if (this->charging_to_floating_mode_) {
          this->charging_to_floating_mode_->publish_state(value_charging_to_floating_mode_);
        }
        if (this->switch_on_) {
          this->switch_on_->publish_state(value_switch_on_);
        }
        if (this->dustproof_installed_) {
          this->dustproof_installed_->publish_state(value_dustproof_installed_);
        }
        if (this->solar_feed_to_grid_status_) {
          this->solar_feed_to_grid_status_->publish_state(value_solar_feed_to_grid_status_);
        }
        if (this->country_customized_regulation_) {
          this->country_customized_regulation_->publish_state(value_country_customized_regulation_);
        }
        if (this->solar_feed_to_grid_power_) {
          this->solar_feed_to_grid_power_->publish_state(value_solar_feed_to_grid_power_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QPIGS2:
        if (this->pv2_input_current_) {
          this->pv2_input_current_->publish_state(value_pv2_input_current_);
        }
        if (this->pv2_input_voltage_) {
          this->pv2_input_voltage_->publish_state(value_pv2_input_voltage_);
        }
        if (this->pv2_charging_power_) {
          this->pv2_charging_power_->publish_state(value_pv2_charging_power_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QPGS0:
        if (this->parallel_num_0_) {
          this->parallel_num_0_->publish_state(value_parallel_num_0_);
        }
        if (this->serial_number_0_) {
          this->serial_number_0_->publish_state(value_serial_number_0_);
        }
        if (this->work_mode_0_) {
          mode = value_work_mode_0_;
          this->work_mode_0_->publish_state(mode);
        }
        if (this->fault_code_0_) {
          this->fault_code_0_->publish_state(value_fault_code_0_);
        }
        if (this->grid_voltage_0_) {
          this->grid_voltage_0_->publish_state(value_grid_voltage_0_);
        }
        if (this->grid_frequency_0_) {
          this->grid_frequency_0_->publish_state(value_grid_frequency_0_);
        }
        if (this->ac_output_voltage_0_) {
          this->ac_output_voltage_0_->publish_state(value_ac_output_voltage_0_);
        }
        if (this->ac_output_frequency_0_) {
          this->ac_output_frequency_0_->publish_state(value_ac_output_frequency_0_);
        }
        if (this->ac_output_apparent_power_0_) {
          this->ac_output_apparent_power_0_->publish_state(value_ac_output_apparent_power_0_);
        }
        if (this->ac_output_active_power_0_) {
          this->ac_output_active_power_0_->publish_state(value_ac_output_active_power_0_);
        }
        if (this->load_percent_0_) {
          this->load_percent_0_->publish_state(value_load_percent_0_);
        }
        if (this->battery_voltage_0_) {
          this->battery_voltage_0_->publish_state(value_battery_voltage_0_);
        }
        if (this->battery_charging_current_0_) {
          this->battery_charging_current_0_->publish_state(value_battery_charging_current_0_);
        }
        if (this->battery_capacity_0_) {
          this->battery_capacity_0_->publish_state(value_battery_capacity_0_);
        }
        if (this->pv1_input_voltage_0_) {
          this->pv1_input_voltage_0_->publish_state(value_pv1_input_voltage_0_);
        }
        if (this->total_charging_current_0_) {
          this->total_charging_current_0_->publish_state(value_total_charging_current_0_);
        }
        if (this->total_ac_output_apparent_power_0_) {
          this->total_ac_output_apparent_power_0_->publish_state(value_total_ac_output_apparent_power_0_);
        }
        if (this->total_output_active_power_0_) {
          this->total_output_active_power_0_->publish_state(value_total_output_active_power_0_);
        }
        if (this->total_ac_output_percentage_0_) {
          this->total_ac_output_percentage_0_->publish_state(value_total_ac_output_percentage_0_);
        }
        if (this->inverter_status_scc_0_) {
          this->inverter_status_scc_0_->publish_state(value_inverter_status_scc_0_);
        }
        if (this->inverter_status_ac_charging_0_) {
          this->inverter_status_ac_charging_0_->publish_state(value_inverter_status_ac_charging_0_);
        }
        if (this->inverter_status_scc_charging_0_) {
          this->inverter_status_scc_charging_0_->publish_state(value_inverter_status_scc_charging_0_);
        }
        if (this->inverter_status_battery_0_) {
          this->inverter_status_battery_0_->publish_state(value_inverter_status_battery_0_);
        }
        if (this->inverter_status_line_0_) {
          this->inverter_status_line_0_->publish_state(value_inverter_status_line_0_);
        }
        if (this->inverter_status_load_0_) {
          this->inverter_status_load_0_->publish_state(value_inverter_status_load_0_);
        }
        if (this->inverter_status_configuration_0_) {
          this->inverter_status_configuration_0_->publish_state(value_inverter_status_configuration_0_);
        }
        if (this->output_mode_0_) {
          this->output_mode_0_->publish_state(value_output_mode_0_);
        }
        if (this->charger_source_priority_0_) {
          this->charger_source_priority_0_->publish_state(value_charger_source_priority_0_);
        }
        if (this->max_charger_current_0_) {
          this->max_charger_current_0_->publish_state(value_max_charger_current_0_);
        }
        if (this->max_charger_range_0_) {
          this->max_charger_range_0_->publish_state(value_max_charger_range_0_);
        }
        if (this->max_ac_charger_current_0_) {
          this->max_ac_charger_current_0_->publish_state(value_max_ac_charger_current_0_);
        }
        if (this->pv1_input_current_0_) {
          this->pv1_input_current_0_->publish_state(value_pv1_input_current_0_);
        }
        if (this->battery_discharge_current_0_) {
          this->battery_discharge_current_0_->publish_state(value_battery_discharge_current_0_);
        }
        if (this->pv2_input_voltage_0_) {
          this->pv2_input_voltage_0_->publish_state(value_pv2_input_voltage_0_);
        }
        if (this->pv2_input_current_0_) {
          this->pv2_input_current_0_->publish_state(value_pv2_input_current_0_);
        }

        this->state_ = STATE_IDLE;
        break;
      case POLLING_QMOD:
        if (this->device_mode_) {
          mode = value_device_mode_;
          this->device_mode_->publish_state(mode);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QFLAG:
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
        if (this->power_saving_) {
          this->power_saving_->publish_state(value_power_saving_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QPIWS:
        if (this->warnings_present_) {
          this->warnings_present_->publish_state(value_warnings_present_);
        }
        if (this->faults_present_) {
          this->faults_present_->publish_state(value_faults_present_);
        }
        if (this->warning_power_loss_) {
          this->warning_power_loss_->publish_state(value_warning_power_loss_);
        }
        if (this->fault_inverter_fault_) {
          this->fault_inverter_fault_->publish_state(value_fault_inverter_fault_);
        }
        if (this->fault_bus_over_) {
          this->fault_bus_over_->publish_state(value_fault_bus_over_);
        }
        if (this->fault_bus_under_) {
          this->fault_bus_under_->publish_state(value_fault_bus_under_);
        }
        if (this->fault_bus_soft_fail_) {
          this->fault_bus_soft_fail_->publish_state(value_fault_bus_soft_fail_);
        }
        if (this->warning_line_fail_) {
          this->warning_line_fail_->publish_state(value_warning_line_fail_);
        }
        if (this->fault_opvshort_) {
          this->fault_opvshort_->publish_state(value_fault_opvshort_);
        }
        if (this->fault_inverter_voltage_too_low_) {
          this->fault_inverter_voltage_too_low_->publish_state(value_fault_inverter_voltage_too_low_);
        }
        if (this->fault_inverter_voltage_too_high_) {
          this->fault_inverter_voltage_too_high_->publish_state(value_fault_inverter_voltage_too_high_);
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
        if (this->warning_battery_derating_) {
          this->warning_battery_derating_->publish_state(value_warning_battery_derating_);
        }
        if (this->warning_over_load_) {
          this->warning_over_load_->publish_state(value_warning_over_load_);
        }
        if (this->warning_eeprom_failed_) {
          this->warning_eeprom_failed_->publish_state(value_warning_eeprom_failed_);
        }
        if (this->fault_inverter_over_current_) {
          this->fault_inverter_over_current_->publish_state(value_fault_inverter_over_current_);
        }
        if (this->fault_inverter_soft_failed_) {
          this->fault_inverter_soft_failed_->publish_state(value_fault_inverter_soft_failed_);
        }
        if (this->fault_self_test_failed_) {
          this->fault_self_test_failed_->publish_state(value_fault_self_test_failed_);
        }
        if (this->fault_op_dc_voltage_over_) {
          this->fault_op_dc_voltage_over_->publish_state(value_fault_op_dc_voltage_over_);
        }
        if (this->fault_battery_open_) {
          this->fault_battery_open_->publish_state(value_fault_battery_open_);
        }
        if (this->fault_current_sensor_failed_) {
          this->fault_current_sensor_failed_->publish_state(value_fault_current_sensor_failed_);
        }
        if (this->fault_battery_short_) {
          this->fault_battery_short_->publish_state(value_fault_battery_short_);
        }
        if (this->warning_power_limit_) {
          this->warning_power_limit_->publish_state(value_warning_power_limit_);
        }
        if (this->warning_pv_voltage_high_) {
          this->warning_pv_voltage_high_->publish_state(value_warning_pv_voltage_high_);
        }
        if (this->fault_mppt_overload_) {
          this->fault_mppt_overload_->publish_state(value_fault_mppt_overload_);
        }
        if (this->warning_mppt_overload_) {
          this->warning_mppt_overload_->publish_state(value_warning_mppt_overload_);
        }
        if (this->warning_battery_too_low_to_charge_) {
          this->warning_battery_too_low_to_charge_->publish_state(value_warning_battery_too_low_to_charge_);
        }
        if (this->fault_dc_dc_over_current_) {
          this->fault_dc_dc_over_current_->publish_state(value_fault_dc_dc_over_current_);
        }
        if (this->fault_code_) {
          this->fault_code_->publish_state(value_fault_code_);
        }
        if (this->warnung_low_pv_energy_) {
          this->warnung_low_pv_energy_->publish_state(value_warnung_low_pv_energy_);
        }
        if (this->warning_high_ac_input_during_bus_soft_start_) {
          this->warning_high_ac_input_during_bus_soft_start_->publish_state(
              value_warning_high_ac_input_during_bus_soft_start_);
        }
        if (this->warning_battery_equalization_) {
          this->warning_battery_equalization_->publish_state(value_warning_battery_equalization_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QBATCD:
        if (this->discharge_onoff_) {
          this->discharge_onoff_->publish_state(value_discharge_onoff_);
        }
        if (this->discharge_with_standby_onoff_) {
          this->discharge_with_standby_onoff_->publish_state(value_discharge_with_standby_onoff_);
        }
        if (this->charge_onoff_) {
          this->charge_onoff_->publish_state(value_charge_onoff_);
        }
        if (this->charging_discharging_control_select_) {
          std::string value = str_snprintf("%d%d%d", 32, value_discharge_onoff_, value_discharge_with_standby_onoff_, value_charge_onoff_);
          this->charging_discharging_control_select_->map_and_publish(value_charging_discharging_control_select_);
        }
        this->state_ = STATE_IDLE;
        break;   
      case POLLING_QT:
        if (this->inverter_date_) {
          this->inverter_date_->publish_state(value_inverter_date_);
        }
        if (this->inverter_time_) {
          this->inverter_time_->publish_state(value_inverter_time_);
        }
      case POLLING_QMN:
        this->state_ = STATE_IDLE;
        break;
      case POLLING_Q1:
        if (this->time_until_absorb_charging_) {
          this->time_until_absorb_charging_->publish_state(value_time_until_absorb_charging_);
        }
        if (this->time_until_float_charging_) {
          this->time_until_float_charging_->publish_state(value_time_until_float_charging_);
        }
        if (this->scc_flag_) {
          this->scc_flag_->publish_state(value_scc_flag_);
        }
        if (this->allow_scc_on_) {
          this->allow_scc_on_->publish_state(value_allow_scc_on_);
        }
        if (this->charge_average_current_) {
          this->charge_average_current_->publish_state(value_charge_average_current_);
        }
        if (this->scc_pwm_temperature_) {
          this->scc_pwm_temperature_->publish_state(value_scc_pwm_temperature_);
        }
        if (this->inverter_temperature_) {
          this->inverter_temperature_->publish_state(value_inverter_temperature_);
        }
        if (this->battery_temperature_) {
          this->battery_temperature_->publish_state(value_battery_temperature_);
        }
        if (this->transformer_temperature_) {
          this->transformer_temperature_->publish_state(value_transformer_temperature_);
        }
        if (this->fan_lock_status_) {
          this->fan_lock_status_->publish_state(value_fan_lock_status_);
        }
        if (this->fan_pwm_speed_) {
          this->fan_pwm_speed_->publish_state(value_fan_pwm_speed_);
        }
        if (this->scc_charge_power_) {
          this->scc_charge_power_->publish_state(value_scc_charge_power_);
        }
        if (this->parallel_warning_) {
          this->parallel_warning_->publish_state(value_parallel_warning_);
        }
        if (this->sync_frequency_) {
          this->sync_frequency_->publish_state(value_sync_frequency_);
        }
        if (this->inverter_charge_status_) {
          this->inverter_charge_status_->publish_state(value_inverter_charge_status_);
        }

        this->state_ = STATE_IDLE;
        break;
      case POLLING_QBMS:
        // special for bms_values select
        if (this->bms_values_select_) {
          std::string value = last_qbms_->state;
          this->bms_values_select_->map_and_publish(value);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QET:
        // total_pv_generated_energy
        if (this->total_pv_generated_energy_) {
          this->total_pv_generated_energy_->publish_state(value_total_pv_generated_energy_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QLT:
        // total_output_load_energy
        if (this->total_output_load_energy_) {
          this->total_output_load_energy_->publish_state(value_total_output_load_energy_);
        }
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QMCHGCR:
        this->state_ = STATE_IDLE;
        break;
      case POLLING_QMUCHGCR:
        this->state_ = STATE_IDLE;
        break;
    }
  }

  if (this->state_ == STATE_POLL_CHECKED) {
    bool enabled = true;
    std::string fc;
    char tmp[PIPSOLAR_READ_BUFFER_LENGTH];
    sprintf(tmp, "%s", this->read_buffer_);
    switch (this->used_polling_commands_[this->last_polling_command_].identifier) {
      case POLLING_QPIRI:
        // (230.0 31.3 230.0 50.0 31.3 7200 7200 48.0 46.0 44.0 56.4 54.0 0 002 060 0 1 3 9 01 0 0 54.0 0 1 480 0 000
        // (BBB.B CC.C DDD.D EE.E FF.F HHHH IIII JJ.J KK.K JJ.J KK.K LL.L O  PP QQ0 O P Q R SS T U VV.V W X YYY Z CCC
        ESP_LOGD(TAG, "Decode QPIRI");
        int operation_logic;
        sscanf(tmp, "(%f %f %f %f %f %d %d %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d %f %d %d %d %d %d", // NOLINT
               &value_grid_rating_voltage_, &value_grid_rating_current_, &value_ac_output_rating_voltage_,  // NOLINT
               &value_ac_output_rating_frequency_, &value_ac_output_rating_current_,                        // NOLINT
               &value_ac_output_rating_apparent_power_, &value_ac_output_rating_active_power_,              // NOLINT
               &value_battery_rating_voltage_, &value_battery_recharge_voltage_,                            // NOLINT
               &value_battery_under_voltage_, &value_battery_bulk_voltage_, &value_battery_float_voltage_,  // NOLINT
               &value_battery_type_, &value_current_max_ac_charging_current_,                               // NOLINT
               &value_current_max_charging_current_, &value_input_voltage_range_,                           // NOLINT
               &value_output_source_priority_, &value_charger_source_priority_, &value_parallel_max_num_,   // NOLINT
               &value_machine_type_, &value_topology_, &value_output_mode_,                                 // NOLINT
               &value_battery_redischarge_voltage_, &value_pv_ok_condition_for_parallel_,                   // NOLINT
               &value_pv_power_balance_, &value_max_charging_time_at_cv_stage_, &operation_logic,           // NOLINT
               &value_max_discharging_current_);                                                            // NOLINT
        //ESP_LOGD(TAG, "operation_logic: %d", operation_logic);
        value_operation_logic_ = operation_logic + '0';
        //ESP_LOGD(TAG, "operation_logic: %c", value_operation_logic_);
        if (this->last_qpiri_) {
          this->last_qpiri_->publish_state(tmp);
        }
        /*
        this->current_max_ac_charging_current_select_ = value_current_max_ac_charging_current_;
        this->current_max_charging_current_select_ = value_current_max_charging_current_;
        */
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QPIGS:
        ESP_LOGD(TAG, "Decode QPIGS");
        // (BBB.B CC.C DDD.D EE.E FFFF GGGG HHH III JJ.JJ KKK OOO TTTT EE.E UUU.U WW.WW PPPPP b7b6b5b4b3b2b1b0 QQ VV MMMMM b10b9b8 Y ZZ AAAA
        sscanf(                                                                                              // NOLINT
            tmp,                                                                                             // NOLINT
            "(%f %f %f %f %d %d %d %d %f %d %d %d %f %f %f %d %1d%1d%1d%1d%1d%1d%1d%1d %d %d %d %1d%1d%1d %d %d %d",  // NOLINT
            &value_grid_voltage_, &value_grid_frequency_, &value_ac_output_voltage_,                         // NOLINT
            &value_ac_output_frequency_,                                                                     // NOLINT
            &value_ac_output_apparent_power_, &value_ac_output_active_power_, &value_output_load_percent_,   // NOLINT
            &value_bus_voltage_, &value_battery_voltage_, &value_battery_charging_current_,                  // NOLINT
            &value_battery_capacity_percent_, &value_inverter_heat_sink_temperature_,                        // NOLINT
            &value_pv1_input_current_, &value_pv1_input_voltage_, &value_battery_voltage_scc_,               // NOLINT
            &value_battery_discharge_current_, &value_add_sbu_priority_version_,                             // NOLINT
            &value_configuration_status_, &value_scc_firmware_version_, &value_load_status_,                 // NOLINT
            &value_battery_voltage_to_steady_while_charging_, &value_charging_status_,                       // NOLINT
            &value_scc_charging_status_, &value_ac_charging_status_,                                         // NOLINT
            &value_battery_voltage_offset_for_fans_on_, &value_eeprom_version_, &value_pv1_charging_power_,  // NOLINT
            &value_charging_to_floating_mode_, &value_switch_on_,                                            // NOLINT
            &value_dustproof_installed_, &value_solar_feed_to_grid_status_,                                  // NOLINT
            &value_country_customized_regulation_, &value_solar_feed_to_grid_power_                          // NOLINT
            );                                                                                               // NOLINT
        if (this->last_qpigs_) {
          this->last_qpigs_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QPIGS2:
        ESP_LOGD(TAG, "Decode QPIGS2");
        sscanf(                                                                                 // NOLINT
            tmp,                                                                                // NOLINT
            "(%f %f %d",                                                                        // NOLINT
            &value_pv2_input_current_, &value_pv2_input_voltage_, &value_pv2_charging_power_);  // NOLINT
        if (this->last_qpigs2_) {
          this->last_qpigs2_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QPGS0:
        ESP_LOGD(TAG, "Decode QPGS0");
        // (A BBBBBBBBBBBBBB C DD EEE.E FF.FF GGG.G HH.HH IIII JJJJ KKK LL.L MMM NNN OOO.O PPP QQQQQ RRRRR SSS b7b6b5b4b3b2b1b0 T U VVV WWW ZZ  XX YYY OOO.O XX
        // (0 92932210103877 L 00 233.5 49.99 233.5 49.99 0747 0641 010 28.4 000 000 000.0 000 00747 00641 008  0 1 0  02 0 1 0 0 3 060 120 002 00 000 000.0 00
        sscanf(                                                                                              // NOLINT
            tmp,                                                                                             // NOLINT
            "(%d %ld %c %d %f %f %f %f %d %d %d %f %d %d %f %d %d %d %d %1d%1d%1d%2d%1d%1d%1d %d %d %d %d %d %d %f %d",  // NOLINT
            &value_parallel_num_0_,                                                                          // NOLINT  A
            &value_serial_number_0_,                                                                         // NOLINT  BBBBBBBBBBBBBB
            &value_work_mode_0_,                                                                             // NOLINT  C
            &value_fault_code_0_,                                                                            // NOLINT  DD
            &value_grid_voltage_0_,                                                                          // NOLINT  EEE.E
            &value_grid_frequency_0_,                                                                        // NOLINT  FF.FF
            &value_ac_output_voltage_0_,                                                                     // NOLINT  GGG.G
            &value_ac_output_frequency_0_,                                                                   // NOLINT  HH.HH
            &value_ac_output_apparent_power_0_,                                                              // NOLINT  IIII
            &value_ac_output_active_power_0_,                                                                // NOLINT  JJJJ
            &value_load_percent_0_,                                                                          // NOLINT  KKK
            &value_battery_voltage_0_,                                                                       // NOLINT  LL.L
            &value_battery_charging_current_0_,                                                              // NOLINT  MMM
            &value_battery_capacity_0_,                                                                      // NOLINT  NNN
            &value_pv1_input_voltage_0_,                                                                     // NOLINT  OOO.O
            &value_total_charging_current_0_,                                                                // NOLINT  PPP
            &value_total_ac_output_apparent_power_0_,                                                        // NOLINT  QQQQQ
            &value_total_output_active_power_0_,                                                             // NOLINT  RRRRR 
            &value_total_ac_output_percentage_0_,                                                            // NOLINT  SSS
            &value_inverter_status_scc_0_,                                                                   // NOLINT  b7
            &value_inverter_status_ac_charging_0_,                                                           // NOLINT  b6
            &value_inverter_status_scc_charging_0_,                                                          // NOLINT  b5
            &value_inverter_status_battery_0_,                                                               // NOLINT  b4b3
            &value_inverter_status_line_0_,                                                                  // NOLINT  b2
            &value_inverter_status_load_0_,                                                                  // NOLINT  b1
            &value_inverter_status_configuration_0_,                                                         // NOLINT  b0
            &value_output_mode_0_,                                                                           // NOLINT  T
            &value_charger_source_priority_0_,                                                               // NOLINT  U
            &value_max_charger_current_0_,                                                                   // NOLINT  VVV
            &value_max_charger_range_0_,                                                                     // NOLINT  WWW
            &value_max_ac_charger_current_0_,                                                                // NOLINT  ZZ 
            &value_pv1_input_current_0_,                                                                     // NOLINT  XX 
            &value_battery_discharge_current_0_,                                                             // NOLINT  YYY 
            &value_pv2_input_voltage_0_,                                                                     // NOLINT  OOO.O 
            &value_pv2_input_current_0_                                                                      // NOLINT  XX
            );                                                                                               // NOLINT
        if (this->last_qpgs0_) {
          this->last_qpgs0_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QMOD:
        ESP_LOGD(TAG, "Decode QMOD");
        this->value_device_mode_ = char(this->read_buffer_[1]);
        if (this->last_qmod_) {
          this->last_qmod_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QFLAG:
        ESP_LOGD(TAG, "Decode QFLAG");
        // result like:"(EbkuvxzDajy"
        // get through all char: ignore first "(" Enable flag on 'E', Disable on 'D') else set the corresponding value
        for (size_t i = 1; i < strlen(tmp); i++) {
          switch (tmp[i]) {
            case 'E':
              enabled = true;
              break;
            case 'D':
              enabled = false;
              break;
            case 'a':
              this->value_silence_buzzer_open_buzzer_ = enabled;
              break;
            case 'b':
              this->value_overload_bypass_function_ = enabled;
              break;
            case 'k':
              this->value_lcd_escape_to_default_ = enabled;
              break;
            case 'u':
              this->value_overload_restart_function_ = enabled;
              break;
            case 'v':
              this->value_over_temperature_restart_function_ = enabled;
              break;
            case 'x':
              this->value_backlight_on_ = enabled;
              break;
            case 'y':
              this->value_alarm_on_when_primary_source_interrupt_ = enabled;
              break;
            case 'z':
              this->value_fault_code_record_ = enabled;
              break;
            case 'j':
              this->value_power_saving_ = enabled;
              break;
          }
        }
        if (this->last_qflag_) {
          this->last_qflag_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QPIWS:
        ESP_LOGD(TAG, "Decode QPIWS");
        // '(00000000000000000000000000000000'
        // iterate over all available flag (as not all models have all flags, but at least in the same order)
        this->value_warnings_present_ = false;
        this->value_faults_present_ = true;

        for (size_t i = 1; i < strlen(tmp); i++) {
          enabled = tmp[i] == '1';
          switch (i) {
            case 1:
              this->value_warning_power_loss_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 2:
              this->value_fault_inverter_fault_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 3:
              this->value_fault_bus_over_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 4:
              this->value_fault_bus_under_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 5:
              this->value_fault_bus_soft_fail_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 6:
              this->value_warning_line_fail_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 7:
              this->value_fault_opvshort_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 8:
              this->value_fault_inverter_voltage_too_low_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 9:
              this->value_fault_inverter_voltage_too_high_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 10:
              this->value_warning_over_temperature_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 11:
              this->value_warning_fan_lock_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 12:
              this->value_warning_battery_voltage_high_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 13:
              this->value_warning_battery_low_alarm_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 15:
              this->value_warning_battery_under_shutdown_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 16:
              this->value_warning_battery_derating_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 17:
              this->value_warning_over_load_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 18:
              this->value_warning_eeprom_failed_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 19:
              this->value_fault_inverter_over_current_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 20:
              this->value_fault_inverter_soft_failed_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 21:
              this->value_fault_self_test_failed_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 22:
              this->value_fault_op_dc_voltage_over_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 23:
              this->value_fault_battery_open_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 24:
              this->value_fault_current_sensor_failed_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 25:
              this->value_fault_battery_short_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 26:
              this->value_warning_power_limit_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 27:
              this->value_warning_pv_voltage_high_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 28:
              this->value_fault_mppt_overload_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 29:
              this->value_warning_mppt_overload_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 30:
              this->value_warning_battery_too_low_to_charge_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 31:
              this->value_fault_dc_dc_over_current_ = enabled;
              this->value_faults_present_ += enabled;
              break;
            case 32:
              fc = tmp[i];
              fc += tmp[i + 1];
              this->value_fault_code_ = parse_number<int>(fc).value_or(0);
              break;
            case 34:
              this->value_warnung_low_pv_energy_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 35:
              this->value_warning_high_ac_input_during_bus_soft_start_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
            case 36:
              this->value_warning_battery_equalization_ = enabled;
              this->value_warnings_present_ += enabled;
              break;
          }
        }
        if (this->last_qpiws_) {
          this->last_qpiws_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QT: {
        
        ESP_LOGD(TAG, "Decode QT");
        // (YYYYMMDDHHMMSS<cr>       

        if (tmp) {
          std::string tmp_buffer_ = tmp;

          std::string year_   = tmp_buffer_.substr(1, 4);
          std::string month_  = tmp_buffer_.substr(5, 2);
          std::string day_    = tmp_buffer_.substr(7, 2);
          std::string hour_   = tmp_buffer_.substr(9, 2);
          std::string minute_ = tmp_buffer_.substr(11, 2);
          std::string second_ = tmp_buffer_.substr(13, 2);
          
          ESP_LOGD(TAG, "QT data loaded: %s => %s-%s-%s %s:%s:%s", tmp, year_.c_str(), month_.c_str(), day_.c_str(), 
                      hour_.c_str(), minute_.c_str(), second_.c_str());
          
          if (this->inverter_date_) {
            std::string date_ = year_ + "-" + month_ + "-" + day_;
            this->value_inverter_date_ = date_;
          }
          if (this->inverter_time_) {
            std::string time_ = hour_ + ":" + minute_ + ":" + second_;
            this->value_inverter_time_ = time_;
          }
        }

        if (this->last_qt_) {
          this->last_qt_->publish_state(tmp);
        }

        this->state_ = STATE_POLL_DECODED;
        break;
      }
      case POLLING_QMN:
        ESP_LOGD(TAG, "Decode QMN");
        if (this->last_qmn_) {
          this->last_qmn_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_Q1:
        ESP_LOGD(TAG, "Decode Q1");
        int gpio13;
        int not_used;
        int inverter_charge_status;
        int unknown_value_in_response_18;
        int unknown_value_in_response_19;
        int unknown_value_in_response_20;
        int unknown_value_in_response_21;
        int unknown_value_in_response_22;
        int unknown_value_in_response_23;
        int unknown_value_in_response_24;
        int unknown_value_in_response_25;
        int unknown_value_in_response_26;
        int unknown_value_in_response_27;
        sscanf(                                                                                              // NOLINT
            tmp,                                                                                             // NOLINT
            // '(00001 00013 00 00 00 019 024 021 017 02 00 000 0030 0406 0000 00.00 10 0 060 030 060 030 58.40 000 120 0 0000
            "(%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %d %d %d %d %d %d %d %d %f %d %d %d %d",       // NOLINT
            &value_time_until_absorb_charging_,                                                              // NOLINT  00001
            &value_time_until_float_charging_,                                                               // NOLINT  00013
            &value_scc_flag_,                                                                                // NOLINT  00
            &value_allow_scc_on_,                                                                            // NOLINT  00
            &value_charge_average_current_,                                                                  // NOLINT  00
            &value_scc_pwm_temperature_,                                                                     // NOLINT  019
            &value_inverter_temperature_,                                                                    // NOLINT  024
            &value_battery_temperature_,                                                                     // NOLINT  021
            &value_transformer_temperature_,                                                                 // NOLINT  017
            &gpio13,                                                                                         // NOLINT  02
            &value_fan_lock_status_,                                                                         // NOLINT  00
            &not_used,                                                                                       // NOLINT  000
            &value_fan_pwm_speed_,                                                                           // NOLINT  0030
            &value_scc_charge_power_,                                                                        // NOLINT  0406
            &value_parallel_warning_,                                                                        // NOLINT  0000
            &value_sync_frequency_,                                                                          // NOLINT  00.00
            &inverter_charge_status,                                                                         // NOLINT  10
            &unknown_value_in_response_18,                                                                   // NOLINT  0
            &unknown_value_in_response_19,                                                                   // NOLINT  060
            &unknown_value_in_response_20,                                                                   // NOLINT  030
            &unknown_value_in_response_21,                                                                   // NOLINT  060
            &unknown_value_in_response_22,                                                                   // NOLINT  030
            &unknown_value_in_response_23,                                                                   // NOLINT  58.40
            &unknown_value_in_response_24,                                                                   // NOLINT  000
            &unknown_value_in_response_25,                                                                   // NOLINT  120
            &unknown_value_in_response_26,                                                                   // NOLINT  0
            &unknown_value_in_response_27                                                                    // NOLINT  0000
            );                                                                                               // NOLINT
        if (this->last_q1_) {
          this->last_q1_->publish_state(tmp);
        }
        value_inverter_charge_status_ = std::to_string(inverter_charge_status);
        //ESP_LOGD(TAG, "Inverter Charge Status (raw) = %d -> %s", inverter_charge_status, value_inverter_charge_status_);
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QBMS:
        ESP_LOGD(TAG, "Decode QBMS");
        int value_bms_battery_connect_status_;
        int value_bms_battery_percentage_;
        int value_bms_force_charging_;
        int value_bms_stop_discharge_flag_;
        int value_bms_stop_charge_flag_;
        int value_bms_bulk_voltage_;
        int value_bms_float_voltage_;
        int value_bms_cutoff_voltage_;
        int value_bms_max_charge_current_;
        int value_bms_max_discharge_current_;
        sscanf(                                                                                              // NOLINT
            tmp,                                                                                             // NOLINT
            // '(a bbb c d e fff ggg hhh iiii jjjj
            //  (1 000 0 0 0 000 000 000 0000 0000
            "(%d %d %d %d %d %d %d %d %d %d",                                                                // NOLINT
            &value_bms_battery_connect_status_,                                                              // NOLINT  0
            &value_bms_battery_percentage_,                                                                  // NOLINT  000
            &value_bms_force_charging_,                                                                      // NOLINT  0
            &value_bms_stop_discharge_flag_,                                                                 // NOLINT  0
            &value_bms_stop_charge_flag_,                                                                    // NOLINT  0
            &value_bms_bulk_voltage_,                                                                        // NOLINT  000
            &value_bms_float_voltage_,                                                                       // NOLINT  000
            &value_bms_cutoff_voltage_,                                                                      // NOLINT  000
            &value_bms_max_charge_current_,                                                                  // NOLINT  000
            &value_bms_max_discharge_current_                                                                // NOLINT  000
            );                                                                                               // NOLINT
        if (this->last_qbms_) {
          this->last_qbms_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QET:
        ESP_LOGD(TAG, "Decode QET");
        //(NNNNNNNN
        sscanf(                                                                                 // NOLINT
            tmp,                                                                                // NOLINT
            "(%d",                                                                              // NOLINT
            &value_total_pv_generated_energy_);                                                 // NOLINT
        if (this->last_qet_) {
          this->last_qet_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QLT:
        ESP_LOGD(TAG, "Decode QLT");
        //(NNNNNNNN
        sscanf(                                                                                 // NOLINT
            tmp,                                                                                // NOLINT
            "(%d",                                                                              // NOLINT
            &value_total_output_load_energy_);                                                  // NOLINT
        if (this->last_qlt_) {
          this->last_qlt_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QBATCD:
        ESP_LOGD(TAG, "Decode QBATCD: %s", tmp);
        // '(000'
        for (size_t i = 1; i < strlen(tmp); i++) {
          enabled = tmp[i] == '1';
          switch (i) {
            case 1:
              this->value_discharge_onoff_ = enabled;
              break;
            case 2:
              this->value_discharge_with_standby_onoff_ = enabled;
              break;
            case 3:
              this->value_charge_onoff_ = enabled;
              break;
          }
        }
        //this->value_charging_discharging_control_select_ = tmp;
        if (this->last_qbatcd_) {
          this->last_qbatcd_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QMCHGCR:
        ESP_LOGD(TAG, "Decode QMCHGCR");
        if (this->last_qmchgcr_) {
          this->last_qmchgcr_->publish_state(tmp);
        }
        this->state_ = STATE_POLL_DECODED;
        break;
      case POLLING_QMUCHGCR:
        ESP_LOGD(TAG, "Decode QMUCHGCR");
        if (this->last_qmuchgcr_) {
          this->last_qmuchgcr_->publish_state(tmp);
        }
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
