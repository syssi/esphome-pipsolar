import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA

DEPENDENCIES = ["uart"]

# P005GS text sensors
CONF_MPPT1_CHARGER_STATUS = "mppt1_charger_status"
CONF_MPPT2_CHARGER_STATUS = "mppt2_charger_status"
CONF_LOAD_CONNECTION = "load_connection"
CONF_BATTERY_POWER_DIRECTION = "battery_power_direction"
CONF_DC_AC_POWER_DIRECTION = "dc_ac_power_direction"
CONF_LINE_POWER_DIRECTION = "line_power_direction"

# P006MOD text sensor
CONF_DEVICE_MODE = "device_mode"

# Raw response text sensors
CONF_LAST_P005GS = "last_p005gs"
CONF_LAST_P007PIRI = "last_p007piri"
CONF_LAST_P006MOD = "last_p006mod"
CONF_LAST_P007FLAG = "last_p007flag"
CONF_LAST_P005FWS = "last_p005fws"
CONF_LAST_P005ET = "last_p005et"

TYPES = [
    CONF_MPPT1_CHARGER_STATUS,
    CONF_MPPT2_CHARGER_STATUS,
    CONF_LOAD_CONNECTION,
    CONF_BATTERY_POWER_DIRECTION,
    CONF_DC_AC_POWER_DIRECTION,
    CONF_LINE_POWER_DIRECTION,
    CONF_DEVICE_MODE,
    CONF_LAST_P005GS,
    CONF_LAST_P007PIRI,
    CONF_LAST_P006MOD,
    CONF_LAST_P007FLAG,
    CONF_LAST_P005FWS,
    CONF_LAST_P005ET,
]

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): text_sensor.text_sensor_schema() for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type in TYPES:
        if type in config:
            conf = config[type]
            var = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(paren, f"set_{type}")(var))
