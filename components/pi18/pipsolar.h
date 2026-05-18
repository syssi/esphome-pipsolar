#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome::pi18 {

class PipsolarSelect;

enum ENUMPollingCommand {
  POLLING_P007PIRI = 0,
  POLLING_P005GS = 1,
  POLLING_P006MOD = 2,
  POLLING_P007FLAG = 3,
  POLLING_P005FWS = 4,
  POLLING_P005ET = 5,
  POLLING_P007PGS0 = 6,
};

struct PollingCommand {
  uint8_t *command;
  uint8_t length = 0;
  uint8_t errors;
  ENUMPollingCommand identifier;
  bool needs_update;
};

struct QFLAGValues {
  esphome::optional<bool> silence_buzzer_open_buzzer;
  esphome::optional<bool> overload_bypass_function;
  esphome::optional<bool> lcd_escape_to_default;
  esphome::optional<bool> overload_restart_function;
  esphome::optional<bool> over_temperature_restart_function;
  esphome::optional<bool> backlight_on;
  esphome::optional<bool> alarm_on_when_primary_source_interrupt;
  esphome::optional<bool> fault_code_record;
  esphome::optional<bool> power_saving;
};

// Poll commands are sent as "^" + polling_command_name + CRC + CR
#define PIPSOLAR_ENTITY_(type, name, polling_command) \
 protected: \
  type *name##_{}; /* NOLINT */ \
\
 public: \
  void set_##name(type *name) { /* NOLINT */ \
    this->name##_ = name; \
    this->add_polling_command_("^" #polling_command, POLLING_##polling_command); \
  }

#define PIPSOLAR_SENSOR(name, polling_command) PIPSOLAR_ENTITY_(sensor::Sensor, name, polling_command)
#define PIPSOLAR_SWITCH(name, polling_command) PIPSOLAR_ENTITY_(switch_::Switch, name, polling_command)
#define PIPSOLAR_BINARY_SENSOR(name, polling_command) \
  PIPSOLAR_ENTITY_(binary_sensor::BinarySensor, name, polling_command)
#define PIPSOLAR_TEXT_SENSOR(name, polling_command) PIPSOLAR_ENTITY_(text_sensor::TextSensor, name, polling_command)
#define PIPSOLAR_SELECT(name, polling_command) PIPSOLAR_ENTITY_(PipsolarSelect, name, polling_command)

class Pipsolar : public uart::UARTDevice, public PollingComponent {
  // ^P005GS — general status (28 comma-separated fields)
  PIPSOLAR_SENSOR(grid_voltage, P005GS)
  PIPSOLAR_SENSOR(grid_frequency, P005GS)
  PIPSOLAR_SENSOR(ac_output_voltage, P005GS)
  PIPSOLAR_SENSOR(ac_output_frequency, P005GS)
  PIPSOLAR_SENSOR(ac_output_apparent_power, P005GS)
  PIPSOLAR_SENSOR(ac_output_active_power, P005GS)
  PIPSOLAR_SENSOR(output_load_percent, P005GS)
  PIPSOLAR_SENSOR(battery_voltage, P005GS)
  PIPSOLAR_SENSOR(battery_voltage_scc, P005GS)
  PIPSOLAR_SENSOR(battery_voltage_scc2, P005GS)
  PIPSOLAR_SENSOR(battery_discharge_current, P005GS)
  PIPSOLAR_SENSOR(battery_charging_current, P005GS)
  PIPSOLAR_SENSOR(battery_capacity_percent, P005GS)
  PIPSOLAR_SENSOR(inverter_heat_sink_temperature, P005GS)
  PIPSOLAR_SENSOR(mppt1_charger_temperature, P005GS)
  PIPSOLAR_SENSOR(mppt2_charger_temperature, P005GS)
  PIPSOLAR_SENSOR(pv1_input_power, P005GS)
  PIPSOLAR_SENSOR(pv2_input_power, P005GS)
  PIPSOLAR_SENSOR(pv1_input_voltage, P005GS)
  PIPSOLAR_SENSOR(pv2_input_voltage, P005GS)
  PIPSOLAR_BINARY_SENSOR(setting_value_configuration_state, P005GS)
  PIPSOLAR_TEXT_SENSOR(mppt1_charger_status, P005GS)
  PIPSOLAR_TEXT_SENSOR(mppt2_charger_status, P005GS)
  PIPSOLAR_TEXT_SENSOR(load_connection, P005GS)
  PIPSOLAR_TEXT_SENSOR(battery_power_direction, P005GS)
  PIPSOLAR_TEXT_SENSOR(dc_ac_power_direction, P005GS)
  PIPSOLAR_TEXT_SENSOR(line_power_direction, P005GS)

  // ^P007PIRI — device ratings (25 fields, ×0.1 for voltages/currents/frequencies)
  PIPSOLAR_SENSOR(grid_rating_voltage, P007PIRI)
  PIPSOLAR_SENSOR(grid_rating_current, P007PIRI)
  PIPSOLAR_SENSOR(ac_output_rating_voltage, P007PIRI)
  PIPSOLAR_SENSOR(ac_output_rating_frequency, P007PIRI)
  PIPSOLAR_SENSOR(ac_output_rating_current, P007PIRI)
  PIPSOLAR_SENSOR(ac_output_rating_apparent_power, P007PIRI)
  PIPSOLAR_SENSOR(ac_output_rating_active_power, P007PIRI)
  PIPSOLAR_SENSOR(battery_rating_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_recharge_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_redischarge_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_under_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_bulk_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_float_voltage, P007PIRI)
  PIPSOLAR_SENSOR(battery_type, P007PIRI)
  PIPSOLAR_SENSOR(current_max_ac_charging_current, P007PIRI)
  PIPSOLAR_SENSOR(current_max_charging_current, P007PIRI)
  PIPSOLAR_SENSOR(input_voltage_range, P007PIRI)
  PIPSOLAR_SENSOR(output_source_priority, P007PIRI)
  PIPSOLAR_SENSOR(charger_source_priority, P007PIRI)
  PIPSOLAR_SENSOR(parallel_max_num, P007PIRI)
  PIPSOLAR_SENSOR(machine_type, P007PIRI)
  PIPSOLAR_SENSOR(topology, P007PIRI)
  PIPSOLAR_SENSOR(output_mode, P007PIRI)
  PIPSOLAR_SENSOR(solar_power_priority, P007PIRI)
  PIPSOLAR_SENSOR(mppt_string, P007PIRI)

  // ^P006MOD — device mode
  PIPSOLAR_TEXT_SENSOR(device_mode, P006MOD)
  PIPSOLAR_SENSOR(device_mode_id, P006MOD)

  // ^P007FLAG — 9 enable/disable flags
  PIPSOLAR_BINARY_SENSOR(silence_buzzer_open_buzzer, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(overload_bypass_function, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(lcd_escape_to_default, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(overload_restart_function, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(over_temperature_restart_function, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(backlight_on, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(alarm_on_when_primary_source_interrupt, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(fault_code_record, P007FLAG)
  PIPSOLAR_BINARY_SENSOR(power_saving, P007FLAG)

  // ^P005FWS — fault/warning status
  PIPSOLAR_SENSOR(fault_code, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_line_fail, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_output_circuit_short, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_over_temperature, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_fan_lock, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_battery_voltage_high, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_battery_low_alarm, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_battery_under_shutdown, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_over_load, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_eeprom_failed, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_power_limit, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_pv1_voltage_high, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_pv2_voltage_high, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_mppt1_overload, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_mppt2_overload, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_scc1_battery_too_low_to_charge, P005FWS)
  PIPSOLAR_BINARY_SENSOR(warning_scc2_battery_too_low_to_charge, P005FWS)

  // ^P005ET — total generated energy (8-digit, kWh)
  PIPSOLAR_SENSOR(total_generated_energy, P005ET)

  // ^P007PGS0 — parallel unit aggregate status
  PIPSOLAR_SENSOR(total_ac_output_apparent_power, P007PGS0)
  PIPSOLAR_SENSOR(total_ac_output_active_power, P007PGS0)
  PIPSOLAR_SENSOR(total_output_load_percent, P007PGS0)
  PIPSOLAR_SENSOR(total_battery_charging_current, P007PGS0)

  // Raw response text sensors (one per command)
  PIPSOLAR_TEXT_SENSOR(last_p005gs, P005GS)
  PIPSOLAR_TEXT_SENSOR(last_p007piri, P007PIRI)
  PIPSOLAR_TEXT_SENSOR(last_p006mod, P006MOD)
  PIPSOLAR_TEXT_SENSOR(last_p007flag, P007FLAG)
  PIPSOLAR_TEXT_SENSOR(last_p005fws, P005FWS)
  PIPSOLAR_TEXT_SENSOR(last_p005et, P005ET)

  // Selects — user defines optionsmap/statusmap in YAML with ^S commands
  PIPSOLAR_SELECT(output_source_priority_select, P007PIRI)
  PIPSOLAR_SELECT(charger_source_priority_select, P007PIRI)
  PIPSOLAR_SELECT(solar_power_priority_select, P007PIRI)
  PIPSOLAR_SELECT(input_voltage_range_select, P007PIRI)
  PIPSOLAR_SELECT(machine_type_select, P007PIRI)
  PIPSOLAR_SELECT(current_max_charging_current_select, P007PIRI)
  PIPSOLAR_SELECT(current_max_ac_charging_current_select, P007PIRI)
  PIPSOLAR_SELECT(battery_under_voltage_select, P007PIRI)

  // Switches — on/off commands are full ^S<len><cmd> strings
  PIPSOLAR_SWITCH(output_source_priority_switch, P007PIRI)
  PIPSOLAR_SWITCH(solar_power_priority_switch, P007PIRI)
  PIPSOLAR_SWITCH(charger_source_priority_solarfirst_switch, P007PIRI)
  PIPSOLAR_SWITCH(charger_source_priority_utility_switch, P007PIRI)
  PIPSOLAR_SWITCH(charger_source_priority_solaronly_switch, P007PIRI)
  PIPSOLAR_SWITCH(silence_buzzer_open_buzzer_switch, P007FLAG)
  PIPSOLAR_SWITCH(overload_bypass_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(lcd_escape_to_default_switch, P007FLAG)
  PIPSOLAR_SWITCH(overload_restart_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(over_temperature_restart_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(backlight_on_switch, P007FLAG)
  PIPSOLAR_SWITCH(alarm_on_when_primary_source_interrupt_switch, P007FLAG)
  PIPSOLAR_SWITCH(fault_code_record_switch, P007FLAG)
  PIPSOLAR_SWITCH(power_saving_switch, P007FLAG)

  void queue_command(const std::string &command);
  void setup() override;
  void loop() override;
  void dump_config() override;
  void update() override;

 protected:
  static const size_t PIPSOLAR_READ_BUFFER_LENGTH = 200;
  static const size_t COMMAND_QUEUE_LENGTH = 10;
  static const size_t COMMAND_TIMEOUT = 5000;
  static const size_t POLLING_COMMANDS_MAX = 10;

  void add_polling_command_(const char *command, ENUMPollingCommand polling_command);
  void empty_uart_buffer_();
  uint8_t check_incoming_crc_();
  uint8_t check_incoming_length_(uint8_t length);
  uint16_t pipsolar_crc_(uint8_t *msg, uint8_t len);
  bool send_next_command_();
  bool send_next_poll_();

  void handle_poll_response_(ENUMPollingCommand polling_command, const char *message);
  void handle_poll_error_(ENUMPollingCommand polling_command);
  void handle_p005gs_(const char *message);
  void handle_p007piri_(const char *message);
  void handle_p006mod_(const char *message);
  void handle_p007flag_(const char *message);
  void handle_p005fws_(const char *message);
  void handle_p005et_(const char *message);
  void handle_p007pgs0_(const char *message);

  void publish_mppt_status_(text_sensor::TextSensor *sensor, int value);
  void publish_direction_(text_sensor::TextSensor *sensor, int value);
  void publish_binary_sensor_(esphome::optional<bool> b, binary_sensor::BinarySensor *sensor);

  void skip_start_(const char *message, size_t *pos);
  void skip_field_(const char *message, size_t *pos);
  std::string read_field_(const char *message, size_t *pos);
  void read_sensor_(const char *message, size_t *pos, sensor::Sensor *sensor, float scale = 1.0f);

  std::string command_queue_[COMMAND_QUEUE_LENGTH];
  uint8_t command_queue_position_ = 0;
  uint8_t read_buffer_[PIPSOLAR_READ_BUFFER_LENGTH];
  size_t read_pos_{0};

  uint32_t command_start_millis_ = 0;
  uint8_t state_;
  enum State {
    STATE_IDLE = 0,
    STATE_POLL = 1,
    STATE_COMMAND = 2,
    STATE_POLL_COMPLETE = 3,
    STATE_COMMAND_COMPLETE = 4,
    STATE_POLL_CHECKED = 5,
  };

  uint8_t last_polling_command_ = 0;
  PollingCommand enabled_polling_commands_[POLLING_COMMANDS_MAX];
};

}  // namespace esphome::pi18

#undef PIPSOLAR_ENTITY_
#undef PIPSOLAR_SENSOR
#undef PIPSOLAR_SWITCH
#undef PIPSOLAR_BINARY_SENSOR
#undef PIPSOLAR_TEXT_SENSOR
#undef PIPSOLAR_SELECT
