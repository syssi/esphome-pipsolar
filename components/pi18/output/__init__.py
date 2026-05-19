from esphome import automation
import esphome.codegen as cg
from esphome.components import output
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_VALUE

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA, pipsolar_ns

DEPENDENCIES = ["pi18"]

PipsolarOutput = pipsolar_ns.class_("PipsolarOutput", output.FloatOutput)
SetOutputAction = pipsolar_ns.class_("SetOutputAction", automation.Action)

CONF_POSSIBLE_VALUES = "possible_values"

CONF_CURRENT_MAX_AC_CHARGING_CURRENT = "current_max_ac_charging_current"
CONF_CURRENT_MAX_CHARGING_CURRENT = "current_max_charging_current"
CONF_BATTERY_BULK_VOLTAGE = "battery_bulk_voltage"
CONF_BATTERY_FLOAT_VOLTAGE = "battery_float_voltage"

_VOLTAGE_STEPS = [round(v * 0.1, 1) for v in range(480, 585)]

TYPES = {
    CONF_CURRENT_MAX_AC_CHARGING_CURRENT: (
        [2, 10, 20, 30, 40, 50, 60, 70, 80, 90],
        "^S014MUCHGC0,%03d",
    ),
    CONF_CURRENT_MAX_CHARGING_CURRENT: (
        [10, 20, 30, 40, 50, 60, 70, 80, 90],
        "^S013MCHGC0,%03d",
    ),
    # ^S015MCHGVmmm.m,nnn.n — bulk and float share the same command;
    # the other voltage is left at its current value by the inverter
    CONF_BATTERY_BULK_VOLTAGE: (
        _VOLTAGE_STEPS,
        "^S015MCHGV%04.1f,%04.1f",
    ),
    CONF_BATTERY_FLOAT_VOLTAGE: (
        _VOLTAGE_STEPS,
        "^S015MCHGV%04.1f,%04.1f",
    ),
}

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(type): output.FLOAT_OUTPUT_SCHEMA.extend(
            {
                cv.Required(CONF_ID): cv.declare_id(PipsolarOutput),
                cv.Optional(CONF_POSSIBLE_VALUES, default=values): cv.All(
                    cv.ensure_list(cv.positive_float), cv.Length(min=1)
                ),
            }
        )
        for type, (values, _) in TYPES.items()
    }
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type, (_, command) in TYPES.items():
        if type in config:
            conf = config[type]
            var = cg.new_Pvariable(conf[CONF_ID])
            await output.register_output(var, conf)
            cg.add(var.set_parent(paren))
            cg.add(var.set_set_command(command))
            if CONF_POSSIBLE_VALUES in conf:
                cg.add(var.set_possible_values(conf[CONF_POSSIBLE_VALUES]))


@automation.register_action(
    "output.pi18.set_level",
    SetOutputAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(PipsolarOutput),
            cv.Required(CONF_VALUE): cv.templatable(cv.positive_float),
        }
    ),
    synchronous=True,
)
async def output_pi18_set_level_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_VALUE], args, cg.float_)
    cg.add(var.set_level(template_))
    return var
