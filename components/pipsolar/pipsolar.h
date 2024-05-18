#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"
#include "esphome/components/pipsolar/select/pipsolar_select.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome {
namespace pipsolar {
class PipsolarSelect;

enum ENUMPollingCommand {
  POLLING_P007PIRI = 0,
  POLLING_P005GS = 1,
  POLLING_P006MOD = 2,
  POLLING_P007FLAG = 3,
  POLLING_P005FWS = 4,
  //            POLLING_QT = 5,
  //            POLLING_QMN = 6,
  POLLING_P007PGS0 = 10,
  POLLING_P005ET = 30,
};

struct PollingCommand {
  uint8_t *command;
  uint8_t length = 0;
  uint8_t errors;
  ENUMPollingCommand identifier;
};

#define PIPSOLAR_VALUED_ENTITY_(type, name, polling_command, value_type) \
 protected: \
  value_type value_##name##_; \
  PIPSOLAR_ENTITY_(type, name, polling_command)

#define PIPSOLAR_ENTITY_(type, name, polling_command) \
 protected: \
  type *name##_{}; /* NOLINT */ \
\
 public: \
  void set_##name(type *name) { /* NOLINT */ \
    this->name##_ = name; \
    this->add_polling_command_("^" #polling_command, POLLING_##polling_command); \
  }

#define PIPSOLAR_SENSOR(name, polling_command, value_type) \
  PIPSOLAR_VALUED_ENTITY_(sensor::Sensor, name, polling_command, value_type)
#define PIPSOLAR_SWITCH(name, polling_command) PIPSOLAR_ENTITY_(switch_::Switch, name, polling_command)
#define PIPSOLAR_SELECT(name, polling_command) PIPSOLAR_ENTITY_(pipsolar::PipsolarSelect, name, polling_command)
#define PIPSOLAR_VALUED_SELECT(name, polling_command, value_type) \
  PIPSOLAR_VALUED_ENTITY_(pipsolar::PipsolarSelect, name, polling_command, value_type)
#define PIPSOLAR_BINARY_SENSOR(name, polling_command, value_type) \
  PIPSOLAR_VALUED_ENTITY_(binary_sensor::BinarySensor, name, polling_command, value_type)
#define PIPSOLAR_VALUED_TEXT_SENSOR(name, polling_command, value_type) \
  PIPSOLAR_VALUED_ENTITY_(text_sensor::TextSensor, name, polling_command, value_type)
#define PIPSOLAR_TEXT_SENSOR(name, polling_command) PIPSOLAR_ENTITY_(text_sensor::TextSensor, name, polling_command)

class Pipsolar : public uart::UARTDevice, public PollingComponent {
  // P005GS values
  PIPSOLAR_SENSOR(grid_voltage, P005GS, float)                            // AAAA
  PIPSOLAR_SENSOR(grid_frequency, P005GS, float)                          // BBB
  PIPSOLAR_SENSOR(ac_output_voltage, P005GS, float)                       // CCCC
  PIPSOLAR_SENSOR(ac_output_frequency, P005GS, float)                     // DDD
  PIPSOLAR_SENSOR(ac_output_apparent_power, P005GS, int)                  // EEEE
  PIPSOLAR_SENSOR(ac_output_active_power, P005GS, int)                    // FFFF
  PIPSOLAR_SENSOR(output_load_percent, P005GS, int)                       // GGG
  PIPSOLAR_SENSOR(battery_voltage, P005GS, float)                         // HHH
  PIPSOLAR_SENSOR(battery_voltage_scc, P005GS, float)                     // III
  PIPSOLAR_SENSOR(battery_voltage_scc2, P005GS, float)                    // JJJ
  PIPSOLAR_SENSOR(battery_discharge_current, P005GS, int)                 // KKK
  PIPSOLAR_SENSOR(battery_charging_current, P005GS, int)                  // LLL
  PIPSOLAR_SENSOR(battery_capacity_percent, P005GS, int)                  // MMM
  PIPSOLAR_SENSOR(inverter_heat_sink_temperature, P005GS, int)            // NNN
  PIPSOLAR_SENSOR(mppt1_charger_temperature, P005GS, float)               // OOO
  PIPSOLAR_SENSOR(mppt2_charger_temperature, P005GS, float)               // PPP
  PIPSOLAR_SENSOR(pv1_input_power, P005GS, float)                         // QQQQ
  PIPSOLAR_SENSOR(pv2_input_power, P005GS, float)                         // RRRR
  PIPSOLAR_SENSOR(pv1_input_voltage, P005GS, float)                       // SSSS
  PIPSOLAR_SENSOR(pv2_input_voltage, P005GS, float)                       // TTTT
  PIPSOLAR_BINARY_SENSOR(setting_value_configuration_state, P005GS, int)  // U
  PIPSOLAR_SENSOR(mppt1_charger_status, P005GS, int)                      // V
  PIPSOLAR_SENSOR(mppt2_charger_status, P005GS, int)                      // W
  PIPSOLAR_BINARY_SENSOR(load_connection, P005GS, int)                    // X

  //PIPSOLAR_SENSOR(battery_power_direction, P005GS, int)                   // Y
  PIPSOLAR_TEXT_SENSOR(battery_power_direction, P005GS)      // Y
  //PIPSOLAR_VALUED_TEXT_SENSOR(battery_power_direction, P005GS, int)      // Y

  PIPSOLAR_SENSOR(dc_ac_power_direction, P005GS, int)                     // Z
  PIPSOLAR_SENSOR(line_power_direction, P005GS, int)                      // a
  PIPSOLAR_SENSOR(local_parallel_id, P005GS, int)                         // b

  PIPSOLAR_SENSOR(total_ac_output_apparent_power, P007PGS0, int)
  PIPSOLAR_SENSOR(total_ac_output_active_power, P007PGS0, int)
  PIPSOLAR_SENSOR(total_output_load_percent, P007PGS0, int)
  PIPSOLAR_SENSOR(total_battery_charging_current, P007PGS0, int)

  // P007PIRI values
  PIPSOLAR_SENSOR(grid_rating_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(grid_rating_current, P007PIRI, float)
  PIPSOLAR_SENSOR(ac_output_rating_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(ac_output_rating_frequency, P007PIRI, float)
  PIPSOLAR_SENSOR(ac_output_rating_current, P007PIRI, float)
  PIPSOLAR_SENSOR(ac_output_rating_apparent_power, P007PIRI, int)
  PIPSOLAR_SENSOR(ac_output_rating_active_power, P007PIRI, int)
  PIPSOLAR_SENSOR(battery_rating_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_recharge_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_redischarge_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_under_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_bulk_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_float_voltage, P007PIRI, float)
  PIPSOLAR_SENSOR(battery_type, P007PIRI, int)
  PIPSOLAR_SENSOR(current_max_ac_charging_current, P007PIRI, int)
  PIPSOLAR_SENSOR(current_max_charging_current, P007PIRI, int)

  PIPSOLAR_SENSOR(input_voltage_range, P007PIRI, int)
  PIPSOLAR_SELECT(input_voltage_range_select, P007PIRI)

  PIPSOLAR_SENSOR(output_source_priority, P007PIRI, int)
  PIPSOLAR_SELECT(output_source_priority_select, P007PIRI)

  PIPSOLAR_SENSOR(charger_source_priority, P007PIRI, int)
  PIPSOLAR_SELECT(charger_source_priority_select, P007PIRI)

  PIPSOLAR_SENSOR(parallel_max_num, P007PIRI, int)

  PIPSOLAR_SENSOR(machine_type, P007PIRI, int)
  PIPSOLAR_SELECT(machine_type_select, P007PIRI)

  PIPSOLAR_SENSOR(topology, P007PIRI, int)
  PIPSOLAR_SENSOR(output_mode, P007PIRI, int)

  PIPSOLAR_SENSOR(solar_power_priority, P007PIRI, int)
  PIPSOLAR_SELECT(solar_power_priority_select, P007PIRI)

  PIPSOLAR_SENSOR(mppt_string, P007PIRI, int)           // 25 a

  PIPSOLAR_SENSOR(total_generated_energy, P005ET, int)
  //            PIPSOLAR_SENSOR(pv_power_balance, P007PIRI, int)

  // P006MOD values
  PIPSOLAR_VALUED_TEXT_SENSOR(device_mode, P006MOD, char)

  // P007FLAG values
  PIPSOLAR_BINARY_SENSOR(silence_buzzer_open_buzzer, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(overload_bypass_function, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(lcd_escape_to_default, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(overload_restart_function, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(over_temperature_restart_function, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(backlight_on, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(alarm_on_when_primary_source_interrupt, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(fault_code_record, P007FLAG, int)
  PIPSOLAR_BINARY_SENSOR(power_saving, P007FLAG, int)

  // P005FWS values
  PIPSOLAR_SENSOR(fault_code, P005FWS, int)
  PIPSOLAR_BINARY_SENSOR(warning_line_fail, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_output_circuit_short, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_over_temperature, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_fan_lock, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_battery_voltage_high, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_battery_low_alarm, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_battery_under_shutdown, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_over_load, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_eeprom_failed, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_power_limit, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_pv1_voltage_high, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_pv2_voltage_high, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_mppt1_overload, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(warning_mppt2_overload, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(scc1_battery_too_low_to_charge, P005FWS, bool)
  PIPSOLAR_BINARY_SENSOR(scc2_battery_too_low_to_charge, P005FWS, bool)

  //            PIPSOLAR_TEXT_SENSOR(last_qpigs, P005GS)
  //            PIPSOLAR_TEXT_SENSOR(last_qpiri, P007PIRI)
  //            PIPSOLAR_TEXT_SENSOR(last_qmod, P006MOD)
  //            PIPSOLAR_TEXT_SENSOR(last_qflag, P007FLAG)
  //            PIPSOLAR_TEXT_SENSOR(last_qpiws, P005FWS)
  //            PIPSOLAR_TEXT_SENSOR(last_qt, QT)
  //            PIPSOLAR_TEXT_SENSOR(last_qmn, QMN)

  PIPSOLAR_SWITCH(silence_buzzer_open_buzzer_switch, P007FLAG)
  PIPSOLAR_SWITCH(overload_bypass_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(lcd_escape_to_default_switch, P007FLAG)
  PIPSOLAR_SWITCH(overload_restart_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(over_temperature_restart_function_switch, P007FLAG)
  PIPSOLAR_SWITCH(backlight_on_switch, P007FLAG)
  PIPSOLAR_SWITCH(alarm_on_when_primary_source_interrupt_switch, P007FLAG)
  PIPSOLAR_SWITCH(fault_code_record_switch, P007FLAG)
  PIPSOLAR_SWITCH(power_saving_switch, P007FLAG)

  void switch_command(const std::string &command);
  void setup() override;
  void loop() override;
  void dump_config() override;
  void update() override;

 protected:
  friend class PipsolarSelect;
  static const size_t PIPSOLAR_READ_BUFFER_LENGTH = 150;  // maximum supported answer length
  static const size_t COMMAND_QUEUE_LENGTH = 10;
  static const size_t COMMAND_TIMEOUT = 5000;
  uint32_t last_poll_ = 0;
  void add_polling_command_(const char *command, ENUMPollingCommand polling_command);
  void empty_uart_buffer_();
  uint8_t check_incoming_crc_();
  uint8_t check_incoming_length_(uint8_t length);
  uint16_t cal_crc_half_(uint8_t *msg, uint8_t len);
  uint8_t send_next_command_();
  void send_next_poll_();
  void queue_command_(const char *command, uint8_t length);
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
    STATE_POLL_DECODED = 6,
  };

  uint8_t last_polling_command_ = 0;
  PollingCommand used_polling_commands_[15];
};

}  // namespace pipsolar
}  // namespace esphome
