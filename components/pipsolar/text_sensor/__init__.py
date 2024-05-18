import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
#from esphome.components import text_sensor

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA


DEPENDENCIES = ["uart"]

CONF_DEVICE_MODE = "device_mode"
CONF_BATTERY_POWER_DIRECTION = "battery_power_direction"
CONF_LOAD_CONNECTION = "load_connection"
CONF_MPPT1_CHARGER_STATUS = "mppt1_charger_status"
CONF_MPPT2_CHARGER_STATUS = "mppt2_charger_status"
CONF_LINE_POWER_DIRECTION = "line_power_direction"
CONF_DC_AC_POWER_DIRECTION = "dc_ac_power_direction"

# CONF_LAST_QPIGS = "last_qpigs"
# CONF_LAST_QPIRI = "last_qpiri"
# CONF_LAST_QMOD = "last_qmod"
# CONF_LAST_QFLAG = "last_qflag"
# CONF_LAST_QPIWS = "last_qpiws"
# CONF_LAST_QT = "last_qt"
# CONF_LAST_QMN = "last_qmn"

TYPES = [
    CONF_DEVICE_MODE,
    CONF_BATTERY_POWER_DIRECTION,
    CONF_LOAD_CONNECTION,
    CONF_MPPT1_CHARGER_STATUS,
    CONF_MPPT2_CHARGER_STATUS,
    CONF_LINE_POWER_DIRECTION,
    CONF_DC_AC_POWER_DIRECTION,
    #    CONF_LAST_QPIGS,
    #    CONF_LAST_QPIRI,
    #    CONF_LAST_QMOD,
    #    CONF_LAST_QFLAG,
    #    CONF_LAST_QPIWS,
    #    CONF_LAST_QT,
    #    CONF_LAST_QMN,
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
