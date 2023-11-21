import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA

DEPENDENCIES = ["uart"]

# QMOD sensors
CONF_DEVICE_MODE = "device_mode"

CONF_LAST_QPIGS = "last_qpigs"
CONF_LAST_QPIGS2 = "last_qpigs2"
CONF_LAST_QPGS0 = "last_qpgs0"
CONF_LAST_QPIRI = "last_qpiri"
CONF_LAST_QMOD = "last_qmod"
CONF_LAST_QFLAG = "last_qflag"
CONF_LAST_QPIWS = "last_qpiws"
CONF_LAST_QT = "last_qt"
CONF_LAST_QMN = "last_qmn"
CONF_LAST_QET = "last_qet"
CONF_LAST_QLT = "last_qlt"
CONF_LAST_QBATCD = "last_qbatcd"
CONF_LAST_QMCHGCR = "last_qmchgcr"
CONF_LAST_QMUCHGCR = "last_qmuchgcr"
CONF_LAST_Q1 = "last_q1"
CONF_LAST_QBMS = "last_qbms"

# QT sensors

CONF_DATE = "inverter_date"
CONF_TIME = "inverter_time"

CONF_OPERATION_LOGIC = "operation_logic"

# QPGS0 sensors
CONF_WORK_MODE_0 = "work_mode_0"

#Q1
CONF_INVERTER_CHARGE_STATUS = "inverter_charge_status"

TYPES = [
    CONF_DEVICE_MODE,
    CONF_LAST_QPIGS,
    CONF_LAST_QPIGS2,
    CONF_LAST_QPGS0,
    CONF_LAST_QPIRI,
    CONF_LAST_QMOD,
    CONF_LAST_QFLAG,
    CONF_LAST_QPIWS,
    CONF_LAST_QT,
    CONF_LAST_QMN,
    CONF_LAST_QET,
    CONF_LAST_QLT,
    CONF_LAST_QBATCD,
    CONF_LAST_QMCHGCR,
    CONF_LAST_QMUCHGCR,
#    CONF_LAST_MUCHGC,
#    CONF_LAST_MCHGC,
    CONF_LAST_Q1,
    CONF_LAST_QBMS,
    CONF_DATE,
    CONF_TIME,
    CONF_OPERATION_LOGIC,
    CONF_WORK_MODE_0,
    CONF_INVERTER_CHARGE_STATUS,
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
