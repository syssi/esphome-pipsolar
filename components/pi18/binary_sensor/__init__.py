import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA

DEPENDENCIES = ["uart"]

# P005GS binary sensors
CONF_SETTING_VALUE_CONFIGURATION_STATE = "setting_value_configuration_state"

# P007FLAG binary sensors
CONF_SILENCE_BUZZER_OPEN_BUZZER = "silence_buzzer_open_buzzer"
CONF_OVERLOAD_BYPASS_FUNCTION = "overload_bypass_function"
CONF_LCD_ESCAPE_TO_DEFAULT = "lcd_escape_to_default"
CONF_OVERLOAD_RESTART_FUNCTION = "overload_restart_function"
CONF_OVER_TEMPERATURE_RESTART_FUNCTION = "over_temperature_restart_function"
CONF_BACKLIGHT_ON = "backlight_on"
CONF_ALARM_ON_WHEN_PRIMARY_SOURCE_INTERRUPT = "alarm_on_when_primary_source_interrupt"
CONF_FAULT_CODE_RECORD = "fault_code_record"
CONF_POWER_SAVING = "power_saving"

# P005FWS binary sensors
CONF_WARNING_LINE_FAIL = "warning_line_fail"
CONF_WARNING_OUTPUT_CIRCUIT_SHORT = "warning_output_circuit_short"
CONF_WARNING_OVER_TEMPERATURE = "warning_over_temperature"
CONF_WARNING_FAN_LOCK = "warning_fan_lock"
CONF_WARNING_BATTERY_VOLTAGE_HIGH = "warning_battery_voltage_high"
CONF_WARNING_BATTERY_LOW_ALARM = "warning_battery_low_alarm"
CONF_WARNING_BATTERY_UNDER_SHUTDOWN = "warning_battery_under_shutdown"
CONF_WARNING_OVER_LOAD = "warning_over_load"
CONF_WARNING_EEPROM_FAILED = "warning_eeprom_failed"
CONF_WARNING_POWER_LIMIT = "warning_power_limit"
CONF_WARNING_PV1_VOLTAGE_HIGH = "warning_pv1_voltage_high"
CONF_WARNING_PV2_VOLTAGE_HIGH = "warning_pv2_voltage_high"
CONF_WARNING_MPPT1_OVERLOAD = "warning_mppt1_overload"
CONF_WARNING_MPPT2_OVERLOAD = "warning_mppt2_overload"
CONF_WARNING_SCC1_BATTERY_TOO_LOW_TO_CHARGE = "warning_scc1_battery_too_low_to_charge"
CONF_WARNING_SCC2_BATTERY_TOO_LOW_TO_CHARGE = "warning_scc2_battery_too_low_to_charge"

TYPES = [
    CONF_SETTING_VALUE_CONFIGURATION_STATE,
    CONF_SILENCE_BUZZER_OPEN_BUZZER,
    CONF_OVERLOAD_BYPASS_FUNCTION,
    CONF_LCD_ESCAPE_TO_DEFAULT,
    CONF_OVERLOAD_RESTART_FUNCTION,
    CONF_OVER_TEMPERATURE_RESTART_FUNCTION,
    CONF_BACKLIGHT_ON,
    CONF_ALARM_ON_WHEN_PRIMARY_SOURCE_INTERRUPT,
    CONF_FAULT_CODE_RECORD,
    CONF_POWER_SAVING,
    CONF_WARNING_LINE_FAIL,
    CONF_WARNING_OUTPUT_CIRCUIT_SHORT,
    CONF_WARNING_OVER_TEMPERATURE,
    CONF_WARNING_FAN_LOCK,
    CONF_WARNING_BATTERY_VOLTAGE_HIGH,
    CONF_WARNING_BATTERY_LOW_ALARM,
    CONF_WARNING_BATTERY_UNDER_SHUTDOWN,
    CONF_WARNING_OVER_LOAD,
    CONF_WARNING_EEPROM_FAILED,
    CONF_WARNING_POWER_LIMIT,
    CONF_WARNING_PV1_VOLTAGE_HIGH,
    CONF_WARNING_PV2_VOLTAGE_HIGH,
    CONF_WARNING_MPPT1_OVERLOAD,
    CONF_WARNING_MPPT2_OVERLOAD,
    CONF_WARNING_SCC1_BATTERY_TOO_LOW_TO_CHARGE,
    CONF_WARNING_SCC2_BATTERY_TOO_LOW_TO_CHARGE,
]

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): binary_sensor.binary_sensor_schema() for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])
    for type in TYPES:
        if type in config:
            conf = config[type]
            var = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(paren, f"set_{type}")(var))
