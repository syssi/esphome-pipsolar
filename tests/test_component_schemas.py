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

    def test_qpigs2_sensors_present(self):
        assert sensor.CONF_PV2_INPUT_CURRENT in sensor.TYPES
        assert sensor.CONF_PV2_INPUT_VOLTAGE in sensor.TYPES
        assert sensor.CONF_PV2_CHARGING_POWER in sensor.TYPES

    def test_sensor_types_count(self):
        assert len(sensor.TYPES) == 47


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

    def test_qpiws_summary_sensors_present(self):
        assert binary_sensor.CONF_WARNINGS_PRESENT in binary_sensor.TYPES
        assert binary_sensor.CONF_FAULTS_PRESENT in binary_sensor.TYPES

    def test_qbatcd_binary_sensors_present(self):
        assert binary_sensor.CONF_DISCHARGE_ONOFF in binary_sensor.TYPES
        assert binary_sensor.CONF_DISCHARGE_WITH_STANDBY_ONOFF in binary_sensor.TYPES
        assert binary_sensor.CONF_CHARGE_ONOFF in binary_sensor.TYPES

    def test_binary_sensor_types_count(self):
        assert len(binary_sensor.TYPES) == 59

    def test_binary_sensor_values_are_strings(self):
        for key in binary_sensor.TYPES:
            assert isinstance(key, str)

    def test_binary_sensor_values_unique(self):
        assert len(binary_sensor.TYPES) == len(set(binary_sensor.TYPES))


# ── pi18 component ────────────────────────────────────────────────────────────

from components.pi18 import (  # noqa: E402
    binary_sensor as pi18_bs,
    sensor as pi18_sensor,
    text_sensor as pi18_ts,  # noqa: E402
)


class TestPi18SensorTypes:
    def test_p005gs_sensors_present(self):
        assert pi18_sensor.CONF_GRID_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_GRID_FREQUENCY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_FREQUENCY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_APPARENT_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_ACTIVE_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_OUTPUT_LOAD_PERCENT in pi18_sensor.TYPES
        assert "battery_voltage" in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_VOLTAGE_SCC in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_VOLTAGE_SCC2 in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_DISCHARGE_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_CHARGING_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_CAPACITY_PERCENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_INVERTER_HEAT_SINK_TEMPERATURE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_MPPT1_CHARGER_TEMPERATURE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_MPPT2_CHARGER_TEMPERATURE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_PV1_INPUT_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_PV2_INPUT_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_PV1_INPUT_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_PV2_INPUT_VOLTAGE in pi18_sensor.TYPES

    def test_p007piri_sensors_present(self):
        assert pi18_sensor.CONF_GRID_RATING_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_GRID_RATING_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_RATING_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_RATING_FREQUENCY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_RATING_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_RATING_APPARENT_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_AC_OUTPUT_RATING_ACTIVE_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_RATING_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_RECHARGE_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_REDISCHARGE_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_UNDER_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_BULK_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_FLOAT_VOLTAGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_BATTERY_TYPE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_CURRENT_MAX_AC_CHARGING_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_CURRENT_MAX_CHARGING_CURRENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_INPUT_VOLTAGE_RANGE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_OUTPUT_SOURCE_PRIORITY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_CHARGER_SOURCE_PRIORITY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_PARALLEL_MAX_NUM in pi18_sensor.TYPES
        assert pi18_sensor.CONF_MACHINE_TYPE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_TOPOLOGY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_OUTPUT_MODE in pi18_sensor.TYPES
        assert pi18_sensor.CONF_SOLAR_POWER_PRIORITY in pi18_sensor.TYPES
        assert pi18_sensor.CONF_MPPT_STRING in pi18_sensor.TYPES

    def test_p005fws_sensor_present(self):
        assert pi18_sensor.CONF_FAULT_CODE in pi18_sensor.TYPES

    def test_p005et_sensor_present(self):
        assert pi18_sensor.CONF_TOTAL_GENERATED_ENERGY in pi18_sensor.TYPES

    def test_p006mod_sensor_present(self):
        assert pi18_sensor.CONF_DEVICE_MODE_ID in pi18_sensor.TYPES

    def test_p007pgs0_sensors_present(self):
        assert pi18_sensor.CONF_TOTAL_AC_OUTPUT_APPARENT_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_TOTAL_AC_OUTPUT_ACTIVE_POWER in pi18_sensor.TYPES
        assert pi18_sensor.CONF_TOTAL_OUTPUT_LOAD_PERCENT in pi18_sensor.TYPES
        assert pi18_sensor.CONF_TOTAL_BATTERY_CHARGING_CURRENT in pi18_sensor.TYPES

    def test_sensor_types_count(self):
        assert len(pi18_sensor.TYPES) == 52

    def test_sensor_keys_unique(self):
        assert len(pi18_sensor.TYPES) == len(set(pi18_sensor.TYPES))


class TestPi18BinarySensorTypes:
    def test_p005gs_binary_sensors_present(self):
        assert pi18_bs.CONF_SETTING_VALUE_CONFIGURATION_STATE in pi18_bs.TYPES

    def test_p007flag_binary_sensors_present(self):
        assert pi18_bs.CONF_SILENCE_BUZZER_OPEN_BUZZER in pi18_bs.TYPES
        assert pi18_bs.CONF_OVERLOAD_BYPASS_FUNCTION in pi18_bs.TYPES
        assert pi18_bs.CONF_LCD_ESCAPE_TO_DEFAULT in pi18_bs.TYPES
        assert pi18_bs.CONF_OVERLOAD_RESTART_FUNCTION in pi18_bs.TYPES
        assert pi18_bs.CONF_OVER_TEMPERATURE_RESTART_FUNCTION in pi18_bs.TYPES
        assert pi18_bs.CONF_BACKLIGHT_ON in pi18_bs.TYPES
        assert pi18_bs.CONF_ALARM_ON_WHEN_PRIMARY_SOURCE_INTERRUPT in pi18_bs.TYPES
        assert pi18_bs.CONF_FAULT_CODE_RECORD in pi18_bs.TYPES
        assert pi18_bs.CONF_POWER_SAVING in pi18_bs.TYPES

    def test_p005fws_binary_sensors_present(self):
        assert pi18_bs.CONF_WARNING_LINE_FAIL in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_OUTPUT_CIRCUIT_SHORT in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_OVER_TEMPERATURE in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_FAN_LOCK in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_BATTERY_VOLTAGE_HIGH in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_BATTERY_LOW_ALARM in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_BATTERY_UNDER_SHUTDOWN in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_OVER_LOAD in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_EEPROM_FAILED in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_POWER_LIMIT in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_PV1_VOLTAGE_HIGH in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_PV2_VOLTAGE_HIGH in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_MPPT1_OVERLOAD in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_MPPT2_OVERLOAD in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_SCC1_BATTERY_TOO_LOW_TO_CHARGE in pi18_bs.TYPES
        assert pi18_bs.CONF_WARNING_SCC2_BATTERY_TOO_LOW_TO_CHARGE in pi18_bs.TYPES

    def test_binary_sensor_types_count(self):
        assert len(pi18_bs.TYPES) == 26

    def test_binary_sensor_values_unique(self):
        assert len(pi18_bs.TYPES) == len(set(pi18_bs.TYPES))


class TestPi18TextSensorTypes:
    def test_p005gs_text_sensors_present(self):
        assert pi18_ts.CONF_MPPT1_CHARGER_STATUS in pi18_ts.TYPES
        assert pi18_ts.CONF_MPPT2_CHARGER_STATUS in pi18_ts.TYPES
        assert pi18_ts.CONF_LOAD_CONNECTION in pi18_ts.TYPES
        assert pi18_ts.CONF_BATTERY_POWER_DIRECTION in pi18_ts.TYPES
        assert pi18_ts.CONF_DC_AC_POWER_DIRECTION in pi18_ts.TYPES
        assert pi18_ts.CONF_LINE_POWER_DIRECTION in pi18_ts.TYPES

    def test_p006mod_text_sensor_present(self):
        assert pi18_ts.CONF_DEVICE_MODE in pi18_ts.TYPES

    def test_raw_response_text_sensors_present(self):
        assert pi18_ts.CONF_LAST_P005GS in pi18_ts.TYPES
        assert pi18_ts.CONF_LAST_P007PIRI in pi18_ts.TYPES
        assert pi18_ts.CONF_LAST_P006MOD in pi18_ts.TYPES
        assert pi18_ts.CONF_LAST_P007FLAG in pi18_ts.TYPES
        assert pi18_ts.CONF_LAST_P005FWS in pi18_ts.TYPES
        assert pi18_ts.CONF_LAST_P005ET in pi18_ts.TYPES

    def test_text_sensor_types_count(self):
        assert len(pi18_ts.TYPES) == 13

    def test_text_sensor_values_unique(self):
        assert len(pi18_ts.TYPES) == len(set(pi18_ts.TYPES))
