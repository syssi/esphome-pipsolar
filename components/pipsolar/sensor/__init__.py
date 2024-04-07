import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    CONF_BUS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_SPEED,
    ICON_CURRENT_AC,
    ICON_BATTERY,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
    UNIT_WATT_HOURS,
    UNIT_MINUTE,
    UNIT_SECOND,
)

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA

DEPENDENCIES = ["uart"]

# QPIRI sensors
CONF_GRID_RATING_VOLTAGE = "grid_rating_voltage"
CONF_GRID_RATING_CURRENT = "grid_rating_current"
CONF_AC_OUTPUT_RATING_VOLTAGE = "ac_output_rating_voltage"
CONF_AC_OUTPUT_RATING_FREQUENCY = "ac_output_rating_frequency"
CONF_AC_OUTPUT_RATING_CURRENT = "ac_output_rating_current"
CONF_AC_OUTPUT_RATING_APPARENT_POWER = "ac_output_rating_apparent_power"
CONF_AC_OUTPUT_RATING_ACTIVE_POWER = "ac_output_rating_active_power"
CONF_BATTERY_RATING_VOLTAGE = "battery_rating_voltage"
CONF_BATTERY_RECHARGE_VOLTAGE = "battery_recharge_voltage"
CONF_BATTERY_UNDER_VOLTAGE = "battery_under_voltage"
CONF_BATTERY_BULK_VOLTAGE = "battery_bulk_voltage"
CONF_BATTERY_FLOAT_VOLTAGE = "battery_float_voltage"
CONF_BATTERY_TYPE = "battery_type"
CONF_CURRENT_MAX_AC_CHARGING_CURRENT = "current_max_ac_charging_current"
CONF_CURRENT_MAX_CHARGING_CURRENT = "current_max_charging_current"
CONF_INPUT_VOLTAGE_RANGE = "input_voltage_range"
CONF_OUTPUT_SOURCE_PRIORITY = "output_source_priority"
CONF_CHARGER_SOURCE_PRIORITY = "charger_source_priority"
CONF_PARALLEL_MAX_NUM = "parallel_max_num"
CONF_MACHINE_TYPE = "machine_type"
CONF_TOPOLOGY = "topology"
CONF_OUTPUT_MODE = "output_mode"
CONF_BATTERY_REDISCHARGE_VOLTAGE = "battery_redischarge_voltage"
CONF_PV_OK_CONDITION_FOR_PARALLEL = "pv_ok_condition_for_parallel"
CONF_PV_POWER_BALANCE = "pv_power_balance"
CONF_MAX_CHARGING_TIME_AT_CV_STAGE = "max_charging_time_at_cv_stage"
CONF_MAX_DISCHARGING_CURRENT       = "max_discharging_current"

# QPIGS sensors

CONF_GRID_VOLTAGE = "grid_voltage"
CONF_GRID_FREQUENCY = "grid_frequency"
CONF_AC_OUTPUT_VOLTAGE = "ac_output_voltage"
CONF_AC_OUTPUT_FREQUENCY = "ac_output_frequency"
CONF_AC_OUTPUT_APPARENT_POWER = "ac_output_apparent_power"
CONF_AC_OUTPUT_ACTIVE_POWER = "ac_output_active_power"
CONF_OUTPUT_LOAD_PERCENT = "output_load_percent"
CONF_BATTERY_CHARGING_CURRENT = "battery_charging_current"
CONF_BATTERY_CAPACITY_PERCENT = "battery_capacity_percent"
CONF_INVERTER_HEAT_SINK_TEMPERATURE = "inverter_heat_sink_temperature"
CONF_PV1_INPUT_CURRENT = "pv1_input_current"
CONF_PV1_INPUT_VOLTAGE = "pv1_input_voltage"
CONF_BATTERY_VOLTAGE_SCC = "battery_voltage_scc"
CONF_BATTERY_DISCHARGE_CURRENT = "battery_discharge_current"
CONF_ADD_SBU_PRIORITY_VERSION = "add_sbu_priority_version"
CONF_CONFIGURATION_STATUS = "configuration_status"
CONF_SCC_FIRMWARE_VERSION = "scc_firmware_version"
CONF_BATTERY_VOLTAGE_OFFSET_FOR_FANS_ON = "battery_voltage_offset_for_fans_on"
CONF_EEPROM_VERSION = "eeprom_version"
CONF_PV1_CHARGING_POWER = "pv1_charging_power"
CONF_SOLAR_FEED_TO_GRID_POWER="solar_feed_to_grid_power"
CONF_COUNTRY_CUSTOMIZED_REGULATION="country_customized_regulation"

# QPIGS2 sensors

CONF_PV2_INPUT_CURRENT  = "pv2_input_current"
CONF_PV2_INPUT_VOLTAGE  = "pv2_input_voltage"
CONF_PV2_CHARGING_POWER = "pv2_charging_power"

# QPGS0 sensors
CONF_SERIAL_NUMBER_0                  = "serial_number_0" 
CONF_FAULT_CODE_0                     = "fault_code_0"  
CONF_GRID_VOLTAGE_0                   = "grid_voltage_0"
CONF_GRID_FREQUENCY_0                 = "grid_frequency_0"
CONF_AC_OUTPUT_VOLTAGE_0              = "ac_output_voltage_0"
CONF_AC_OUTPUT_FREQUENCY_0            = "ac_output_frequency_0"
CONF_AC_OUTPUT_APPARENT_POWER_0       = "ac_output_apparent_power_0"
CONF_AC_OUTPUT_ACTIVE_POWER_0         = "ac_output_active_power_0"  
CONF_LOAD_PERCENT_0                   = "load_percent_0"
CONF_BATTERY_VOLTAGE_0                = "battery_voltage_0"  
CONF_BATTERY_CHARGING_CURRENT_0       = "battery_charging_current_0"
CONF_BATTERY_CAPACITY_0               = "battery_capacity_0" 
CONF_PV1_INPUT_VOLTAGE_0              = "pv1_input_voltage_0"
CONF_TOTAL_CHARGING_CURRENT_0         = "total_charging_current_0"  
CONF_TOTAL_AC_OUTPUT_APPARENT_POWER_0 = "total_ac_output_apparent_power_0"
CONF_TOTAL_OUTPUT_ACTIVE_POWER_0      = "total_output_active_power_0" 
CONF_TOTAL_AC_OUTPUT_PERCENTAGE_0     = "total_ac_output_percentage_0"
CONF_INVERTER_STATUS_BATTERY_0        = "inverter_status_battery_0"
CONF_OUTPUT_MODE_0                    = "output_mode_0"
CONF_CHARGER_SOURCE_PRIORITY_0        = "charger_source_priority_0"
CONF_MAX_CHARGER_CURRENT_0            = "max_charger_current_0"
CONF_MAX_CHARGER_RANGE_0              = "max_charger_range_0"
CONF_MAX_AC_CHARGER_CURRENT_0         = "max_ac_charger_current_0"
CONF_PV1_INPUT_CURRENT_0              = "pv1_input_current_0"
CONF_BATTERY_DISCHARGE_CURRENT_0      = "battery_discharge_current_0"
CONF_PV2_INPUT_VOLTAGE_0              = "pv2_input_voltage_0"
CONF_PV2_INPUT_CURRENT_0              = "pv2_input_current_0"

# QET sensors
CONF_TOTAL_PV_GENERATED_ENERGY="total_pv_generated_energy"

# QLT sensors
CONF_TOTAL_OUTPUT_LOAD_ENERGY="total_output_load_energy"

# Q1
CONF_TIME_UNTIL_ABSORB_CHARGING = "time_until_absorb_charging"
CONF_TIME_UNTIL_FLOAT_CHARGING  = "time_until_float_charging"
CONF_CHARGE_AVERAGE_CURRENT     = "charge_average_current"
CONF_SCC_PWM_TEMPERATURE        = "scc_pwm_temperature"
CONF_INVERTER_TEMPERATURE       = "inverter_temperature"
CONF_BATTERY_TEMPERATURE        = "battery_temperature"
CONF_TRANSFORMER_TEMPERATURE    = "transformer_temperature"
CONF_FAN_PWM_SPEED              = "fan_pwm_speed"
CONF_SCC_CHARGE_POWER           = "scc_charge_power"
CONF_SYNC_FREQUENCY             = "sync_frequency"

CONF_FAULT_CODE = "fault_code"

TYPES = {
    CONF_GRID_RATING_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_GRID_RATING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_AC_OUTPUT_RATING_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_AC_OUTPUT_RATING_FREQUENCY: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_FREQUENCY,
    ),
    CONF_AC_OUTPUT_RATING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_AC_OUTPUT_RATING_APPARENT_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT_AMPS,
        accuracy_decimals=1,
    ),
    CONF_AC_OUTPUT_RATING_ACTIVE_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_BATTERY_RATING_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_RECHARGE_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_UNDER_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_BULK_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_FLOAT_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_TYPE: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_CURRENT_MAX_AC_CHARGING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_CURRENT_MAX_CHARGING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_INPUT_VOLTAGE_RANGE: sensor.sensor_schema(
        accuracy_decimals=1,
    ),
    CONF_OUTPUT_SOURCE_PRIORITY: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_CHARGER_SOURCE_PRIORITY: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_PARALLEL_MAX_NUM: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_MACHINE_TYPE: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_TOPOLOGY: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_OUTPUT_MODE: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_BATTERY_REDISCHARGE_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_PV_OK_CONDITION_FOR_PARALLEL: sensor.sensor_schema(
        accuracy_decimals=1,
    ),
    CONF_PV_POWER_BALANCE: sensor.sensor_schema(
        accuracy_decimals=1,
    ),
    CONF_GRID_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_GRID_FREQUENCY: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_FREQUENCY,
    ),
    CONF_AC_OUTPUT_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_AC_OUTPUT_FREQUENCY: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_FREQUENCY,
    ),
    CONF_AC_OUTPUT_APPARENT_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT_AMPS,
        accuracy_decimals=1,
    ),
    CONF_AC_OUTPUT_ACTIVE_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_OUTPUT_LOAD_PERCENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=1,
    ),
    CONF_BUS_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_CHARGING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_BATTERY_CAPACITY_PERCENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=1,
    ),
    CONF_INVERTER_HEAT_SINK_TEMPERATURE: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
    ),
    CONF_PV1_INPUT_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_PV1_INPUT_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_VOLTAGE_SCC: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_DISCHARGE_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_BATTERY_VOLTAGE_OFFSET_FOR_FANS_ON: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_EEPROM_VERSION: sensor.sensor_schema(
        accuracy_decimals=1,
    ),
    CONF_PV1_CHARGING_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_PV2_INPUT_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_PV2_INPUT_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_PV2_CHARGING_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_SOLAR_FEED_TO_GRID_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_COUNTRY_CUSTOMIZED_REGULATION: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_TOTAL_PV_GENERATED_ENERGY: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT_HOURS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_ENERGY,
    ),
    CONF_TOTAL_OUTPUT_LOAD_ENERGY: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT_HOURS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_ENERGY,
    ),
    CONF_MAX_CHARGING_TIME_AT_CV_STAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_MINUTE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_DURATION,
    ),
    CONF_MAX_DISCHARGING_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_SERIAL_NUMBER_0: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_FAULT_CODE_0: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_GRID_VOLTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_GRID_FREQUENCY_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        accuracy_decimals=2,
        icon=ICON_CURRENT_AC,
    ),
    CONF_AC_OUTPUT_VOLTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_AC_OUTPUT_FREQUENCY_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        accuracy_decimals=2,
        icon=ICON_CURRENT_AC,
        device_class=DEVICE_CLASS_FREQUENCY,
    ),
    CONF_AC_OUTPUT_APPARENT_POWER_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT_AMPS,
        accuracy_decimals=0,
    ),
    CONF_AC_OUTPUT_ACTIVE_POWER_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_LOAD_PERCENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=0,
    ),
    CONF_BATTERY_VOLTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_BATTERY_CHARGING_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_BATTERY_CAPACITY_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=0,
        icon=ICON_BATTERY,
    ),
    CONF_PV1_INPUT_VOLTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_TOTAL_CHARGING_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_TOTAL_AC_OUTPUT_APPARENT_POWER_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT_AMPS,
        accuracy_decimals=0,
    ),
    CONF_TOTAL_OUTPUT_ACTIVE_POWER_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_TOTAL_AC_OUTPUT_PERCENTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=0,
    ),
    CONF_INVERTER_STATUS_BATTERY_0: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_OUTPUT_MODE_0: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_CHARGER_SOURCE_PRIORITY_0: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
    CONF_MAX_CHARGER_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_MAX_CHARGER_RANGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_MAX_AC_CHARGER_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_PV1_INPUT_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_BATTERY_DISCHARGE_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_PV2_INPUT_VOLTAGE_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_PV2_INPUT_CURRENT_0: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_VOLTAGE,
    ),
    CONF_TIME_UNTIL_ABSORB_CHARGING: sensor.sensor_schema(
        unit_of_measurement=UNIT_SECOND,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_DURATION,
    ),
    CONF_TIME_UNTIL_FLOAT_CHARGING: sensor.sensor_schema(
        unit_of_measurement=UNIT_SECOND,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_DURATION,
    ),
    CONF_CHARGE_AVERAGE_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_CURRENT,
    ),
    CONF_SCC_PWM_TEMPERATURE: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
    ),
    CONF_INVERTER_TEMPERATURE: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
    ),
    CONF_BATTERY_TEMPERATURE: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
    ),
    CONF_TRANSFORMER_TEMPERATURE: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_TEMPERATURE,
    ),
    CONF_FAN_PWM_SPEED: sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_SPEED,
    ),
    CONF_SCC_CHARGE_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_POWER,
    ),
    CONF_SYNC_FREQUENCY: sensor.sensor_schema(
        unit_of_measurement=UNIT_HERTZ,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_FREQUENCY,
    ),
    CONF_FAULT_CODE: sensor.sensor_schema(
        accuracy_decimals=0,
    ),
}

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): schema for type, schema in TYPES.items()}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type, _ in TYPES.items():
        if type in config:
            conf = config[type]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(paren, f"set_{type}")(sens))
