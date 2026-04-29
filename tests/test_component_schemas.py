"""Schema structure tests for pip8048 ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from esphome.const import CONF_BATTERY_VOLTAGE, CONF_BUS_VOLTAGE  # noqa: E402

from components.pip8048 import binary_sensor, sensor  # noqa: E402
from components.pip8048.const import CONF_OUTPUT_SOURCE_PRIORITY  # noqa: E402


class TestSensorTypes:
    def test_qpiri_sensors_present(self):
        assert sensor.CONF_GRID_RATING_VOLTAGE in sensor.TYPES
        assert sensor.CONF_GRID_RATING_CURRENT in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_RATING_VOLTAGE in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_RATING_FREQUENCY in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_RATING_CURRENT in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_RATING_APPARENT_POWER in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_RATING_ACTIVE_POWER in sensor.TYPES
        assert sensor.CONF_BATTERY_RATING_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_RECHARGE_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_UNDER_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_BULK_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_FLOAT_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_TYPE in sensor.TYPES
        assert sensor.CONF_CURRENT_MAX_AC_CHARGING_CURRENT in sensor.TYPES
        assert sensor.CONF_CURRENT_MAX_CHARGING_CURRENT in sensor.TYPES
        assert sensor.CONF_INPUT_VOLTAGE_RANGE in sensor.TYPES
        assert CONF_OUTPUT_SOURCE_PRIORITY in sensor.TYPES
        assert sensor.CONF_CHARGER_SOURCE_PRIORITY in sensor.TYPES
        assert sensor.CONF_PARALLEL_MAX_NUM in sensor.TYPES
        assert sensor.CONF_MACHINE_TYPE in sensor.TYPES
        assert sensor.CONF_TOPOLOGY in sensor.TYPES
        assert sensor.CONF_OUTPUT_MODE in sensor.TYPES
        assert sensor.CONF_BATTERY_REDISCHARGE_VOLTAGE in sensor.TYPES
        assert sensor.CONF_PV_OK_CONDITION_FOR_PARALLEL in sensor.TYPES
        assert sensor.CONF_PV_POWER_BALANCE in sensor.TYPES
        assert sensor.CONF_MAX_CHARGING_TIME_AT_CV_STAGE in sensor.TYPES
        assert sensor.CONF_MAX_DISCHARGING_CURRENT in sensor.TYPES

    def test_qpigs_sensors_present(self):
        assert sensor.CONF_GRID_VOLTAGE in sensor.TYPES
        assert sensor.CONF_GRID_FREQUENCY in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_VOLTAGE in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_FREQUENCY in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_APPARENT_POWER in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_ACTIVE_POWER in sensor.TYPES
        assert sensor.CONF_OUTPUT_LOAD_PERCENT in sensor.TYPES
        assert CONF_BUS_VOLTAGE in sensor.TYPES
        assert CONF_BATTERY_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_CHARGING_CURRENT in sensor.TYPES
        assert sensor.CONF_BATTERY_CAPACITY_PERCENT in sensor.TYPES
        assert sensor.CONF_INVERTER_HEAT_SINK_TEMPERATURE in sensor.TYPES
        assert sensor.CONF_PV1_INPUT_CURRENT in sensor.TYPES
        assert sensor.CONF_PV1_INPUT_VOLTAGE in sensor.TYPES
        assert sensor.CONF_BATTERY_VOLTAGE_SCC in sensor.TYPES
        assert sensor.CONF_BATTERY_DISCHARGE_CURRENT in sensor.TYPES
        assert sensor.CONF_BATTERY_VOLTAGE_OFFSET_FOR_FANS_ON in sensor.TYPES
        assert sensor.CONF_EEPROM_VERSION in sensor.TYPES
        assert sensor.CONF_PV1_CHARGING_POWER in sensor.TYPES
        assert sensor.CONF_SOLAR_FEED_TO_GRID_POWER in sensor.TYPES
        assert sensor.CONF_COUNTRY_CUSTOMIZED_REGULATION in sensor.TYPES

    def test_qpigs2_sensors_present(self):
        assert sensor.CONF_PV2_INPUT_CURRENT in sensor.TYPES
        assert sensor.CONF_PV2_INPUT_VOLTAGE in sensor.TYPES
        assert sensor.CONF_PV2_CHARGING_POWER in sensor.TYPES

    def test_qet_qlt_sensors_present(self):
        assert sensor.CONF_TOTAL_PV_GENERATED_ENERGY in sensor.TYPES
        assert sensor.CONF_TOTAL_OUTPUT_LOAD_ENERGY in sensor.TYPES

    def test_qpgs0_sensors_present(self):
        assert sensor.CONF_SERIAL_NUMBER_0 in sensor.TYPES
        assert sensor.CONF_FAULT_CODE_0 in sensor.TYPES
        assert sensor.CONF_GRID_VOLTAGE_0 in sensor.TYPES
        assert sensor.CONF_GRID_FREQUENCY_0 in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_VOLTAGE_0 in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_FREQUENCY_0 in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_APPARENT_POWER_0 in sensor.TYPES
        assert sensor.CONF_AC_OUTPUT_ACTIVE_POWER_0 in sensor.TYPES
        assert sensor.CONF_LOAD_PERCENT_0 in sensor.TYPES
        assert sensor.CONF_BATTERY_VOLTAGE_0 in sensor.TYPES
        assert sensor.CONF_BATTERY_CHARGING_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_BATTERY_CAPACITY_0 in sensor.TYPES
        assert sensor.CONF_PV1_INPUT_VOLTAGE_0 in sensor.TYPES
        assert sensor.CONF_TOTAL_CHARGING_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_TOTAL_AC_OUTPUT_APPARENT_POWER_0 in sensor.TYPES
        assert sensor.CONF_TOTAL_OUTPUT_ACTIVE_POWER_0 in sensor.TYPES
        assert sensor.CONF_TOTAL_AC_OUTPUT_PERCENTAGE_0 in sensor.TYPES
        assert sensor.CONF_INVERTER_STATUS_BATTERY_0 in sensor.TYPES
        assert sensor.CONF_OUTPUT_MODE_0 in sensor.TYPES
        assert sensor.CONF_CHARGER_SOURCE_PRIORITY_0 in sensor.TYPES
        assert sensor.CONF_MAX_CHARGER_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_MAX_CHARGER_RANGE_0 in sensor.TYPES
        assert sensor.CONF_MAX_AC_CHARGER_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_PV1_INPUT_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_BATTERY_DISCHARGE_CURRENT_0 in sensor.TYPES
        assert sensor.CONF_PV2_INPUT_VOLTAGE_0 in sensor.TYPES
        assert sensor.CONF_PV2_INPUT_CURRENT_0 in sensor.TYPES

    def test_q1_sensors_present(self):
        assert sensor.CONF_TIME_UNTIL_ABSORB_CHARGING in sensor.TYPES
        assert sensor.CONF_TIME_UNTIL_FLOAT_CHARGING in sensor.TYPES
        assert sensor.CONF_CHARGE_AVERAGE_CURRENT in sensor.TYPES
        assert sensor.CONF_SCC_PWM_TEMPERATURE in sensor.TYPES
        assert sensor.CONF_INVERTER_TEMPERATURE in sensor.TYPES
        assert sensor.CONF_BATTERY_TEMPERATURE in sensor.TYPES
        assert sensor.CONF_TRANSFORMER_TEMPERATURE in sensor.TYPES
        assert sensor.CONF_FAN_PWM_SPEED in sensor.TYPES
        assert sensor.CONF_SCC_CHARGE_POWER in sensor.TYPES
        assert sensor.CONF_SYNC_FREQUENCY in sensor.TYPES

    def test_sensor_types_count(self):
        assert len(sensor.TYPES) == 90


class TestBinarySensorTypes:
    def test_qpigs_binary_sensors_present(self):
        assert binary_sensor.CONF_ADD_SBU_PRIORITY_VERSION in binary_sensor.TYPES
        assert binary_sensor.CONF_CONFIGURATION_STATUS in binary_sensor.TYPES
        assert binary_sensor.CONF_SCC_FIRMWARE_VERSION in binary_sensor.TYPES
        assert binary_sensor.CONF_LOAD_STATUS in binary_sensor.TYPES
        assert (
            binary_sensor.CONF_BATTERY_VOLTAGE_TO_STEADY_WHILE_CHARGING
            in binary_sensor.TYPES
        )
        assert binary_sensor.CONF_CHARGING_STATUS in binary_sensor.TYPES
        assert binary_sensor.CONF_SCC_CHARGING_STATUS in binary_sensor.TYPES
        assert binary_sensor.CONF_AC_CHARGING_STATUS in binary_sensor.TYPES
        assert binary_sensor.CONF_CHARGING_TO_FLOATING_MODE in binary_sensor.TYPES
        assert binary_sensor.CONF_SWITCH_ON in binary_sensor.TYPES
        assert binary_sensor.CONF_DUSTPROOF_INSTALLED in binary_sensor.TYPES
        assert binary_sensor.CONF_SOLAR_FEED_TO_GRID_STATUS in binary_sensor.TYPES

    def test_qflag_binary_sensors_present(self):
        assert binary_sensor.CONF_SILENCE_BUZZER_OPEN_BUZZER in binary_sensor.TYPES
        assert binary_sensor.CONF_OVERLOAD_BYPASS_FUNCTION in binary_sensor.TYPES
        assert binary_sensor.CONF_LCD_ESCAPE_TO_DEFAULT in binary_sensor.TYPES
        assert binary_sensor.CONF_OVERLOAD_RESTART_FUNCTION in binary_sensor.TYPES
        assert (
            binary_sensor.CONF_OVER_TEMPERATURE_RESTART_FUNCTION in binary_sensor.TYPES
        )
        assert binary_sensor.CONF_BACKLIGHT_ON in binary_sensor.TYPES
        assert (
            binary_sensor.CONF_ALARM_ON_WHEN_PRIMARY_SOURCE_INTERRUPT
            in binary_sensor.TYPES
        )
        assert binary_sensor.CONF_FAULT_CODE_RECORD in binary_sensor.TYPES
        assert binary_sensor.CONF_POWER_SAVING in binary_sensor.TYPES

    def test_qpiws_binary_sensors_present(self):
        assert binary_sensor.CONF_WARNINGS_PRESENT in binary_sensor.TYPES
        assert binary_sensor.CONF_FAULTS_PRESENT in binary_sensor.TYPES
        assert binary_sensor.CONF_WARNING_BATTERY_WEAK in binary_sensor.TYPES

    def test_qbatcd_binary_sensors_present(self):
        assert binary_sensor.CONF_DISCHARGE_ONOFF in binary_sensor.TYPES
        assert binary_sensor.CONF_DISCHARGE_WITH_STANDBY_ONOFF in binary_sensor.TYPES
        assert binary_sensor.CONF_CHARGE_ONOFF in binary_sensor.TYPES

    def test_qpgs0_binary_sensors_present(self):
        assert binary_sensor.CONF_PARALLEL_NUM_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_SCC_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_AC_CHARGING_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_SCC_CHARGING_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_LINE_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_LOAD_0 in binary_sensor.TYPES
        assert binary_sensor.CONF_INVERTER_STATUS_CONFIGURATION_0 in binary_sensor.TYPES

    def test_q1_binary_sensors_present(self):
        assert binary_sensor.CONF_SCC_FLAG in binary_sensor.TYPES
        assert binary_sensor.CONF_ALLOW_SCC_ON in binary_sensor.TYPES
        assert binary_sensor.CONF_FAN_LOCK_STATUS in binary_sensor.TYPES
        assert binary_sensor.CONF_PARALLEL_WARNING in binary_sensor.TYPES

    def test_binary_sensor_types_count(self):
        assert len(binary_sensor.TYPES) == 71

    def test_binary_sensor_values_are_strings(self):
        for key in binary_sensor.TYPES:
            assert isinstance(key, str)

    def test_binary_sensor_values_unique(self):
        assert len(binary_sensor.TYPES) == len(set(binary_sensor.TYPES))
