import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import output
from esphome.const import CONF_ID, CONF_VALUE
from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA, pipsolar_ns

DEPENDENCIES = ["pipsolar"]

PipsolarOutput = pipsolar_ns.class_("PipsolarOutput", output.FloatOutput)
SetOutputAction = pipsolar_ns.class_("SetOutputAction", automation.Action)

CONF_POSSIBLE_VALUES = "possible_values"

# 3.11 PCVV<nn.n><cr>: Setting battery C.V. (constant voltage) charging voltage 48.0V ~ 58.4V for 48V unit
# battery_bulk_voltage; 
# battery_recharge_voltage;     12V unit: 11V/11.3V/11.5V/11.8V/12V/12.3V/12.5V/12.8V
#                               24V unit: 22V/22.5V/23V/23.5V/24V/24.5V/25V/25.5V
#                               48V unit: 44V/45V/46V/47V/48V/49V/50V/51V
# battery_under_voltage;        40.0V ~ 48.0V for 48V unit
# battery_float_voltage;        48.0V ~ 58.4V for 48V unit
# battery_type;  00 for AGM, 01 for Flooded battery
# current_max_ac_charging_current;
# output_source_priority; 00 / 01 / 02
# charger_source_priority;  For HS: 00 for utility first, 01 for solar first, 02 for solar and utility, 03 for only solar charging
#                           For MS/MSX: 00 for utility first, 01 for solar first, 03 for only solar charging
# battery_redischarge_voltage;  12V unit: 00.0V12V/12.3V/12.5V/12.8V/13V/13.3V/13.5V/13.8V/14V/14.3V/14.5
#                               24V unit: 00.0V/24V/24.5V/25V/25.5V/26V/26.5V/27V/27.5V/28V/28.5V/29V
#                               48V unit: 00.0V48V/49V/50V/51V/52V/53V/54V/55V/56V/57V/58V

# solar_power_priority; 0 Battery-Load-Utiliy, 1 Load-Battery-Utiliy
# machine_type; 0 Off-Grid, 1 Grid-Tie
# battery_power_direction; 0 donothing, 1 charge, 2 discharge

CONF_CURRENT_MAX_AC_CHARGING_CURRENT = "current_max_ac_charging_current"
CONF_CURRENT_MAX_CHARGING_CURRENT = "current_max_charging_current"
CONF_BATTERY_BULK_VOLTAGE = "battery_bulk_voltage"

# CONF_BATTERY_RECHARGE_VOLTAGE = "battery_recharge_voltage"
CONF_BATTERY_UNDER_VOLTAGE = "battery_under_voltage"
# CONF_BATTERY_FLOAT_VOLTAGE = "battery_float_voltage"
# CONF_BATTERY_TYPE = "battery_type"
#
# CONF_OUTPUT_SOURCE_PRIORITY = "output_source_priority"
# CONF_CHARGER_SOURCE_PRIORITY = "charger_source_priority"
# CONF_SOLAR_POWER_PRIORITY = "solar_power_priority"
# CONF_MACHINE_TYPE = "machine_type"
# CONF_BATTERY_POWER_DIRECTION = "battery_power_direction"
# CONF_BATTERY_REDISCHARGE_VOLTAGE = "battery_redischarge_voltage"

TYPES = {
    CONF_CURRENT_MAX_AC_CHARGING_CURRENT: (
        [2, 10, 20, 30, 40, 50, 60, 70, 80, 90],
        "^S013MUCHGC0,%02d",
    ),
    CONF_CURRENT_MAX_CHARGING_CURRENT: (
        [10, 20, 30, 40, 50, 60, 70, 80, 90],
        "^S013MCHGC0,%02d",
    ),
    CONF_BATTERY_BULK_VOLTAGE: (
        [44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0],
        "MCHGV%02.1f",
    ),
    #    CONF_BATTERY_RECHARGE_VOLTAGE: (
    #        [440, 450, 460, 470, 480, 490, 500, 510],
    #        "PBCV%02.1f",
    #    ),
    #    CONF_BATTERY_REDISCHARGE_VOLTAGE: (
    #        [0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58],
    #        "PBDV%02.1f",
    #    ),
    CONF_BATTERY_UNDER_VOLTAGE: (
        [40.0, 40.1, 42, 43, 44, 45, 46, 47, 48.0],
        "PSDV%02.1f",
    ),
    #    CONF_BATTERY_FLOAT_VOLTAGE: ([48.0, 49.0, 50.0, 51.0], "PBFT%02.1f"),
    #    CONF_BATTERY_TYPE: ([0, 1, 2], "PBT%02.0f"),
    #    CONF_OUTPUT_SOURCE_PRIORITY: ([0, 1, 2], "POP%02.0f"),
    #    CONF_SOLAR_POWER_PRIORITY: ([0, 1], "PSP%02.0f"),
    #    CONF_MACHINE_TYPE: ([0, 1], "PDI%02.0f| PEI%02.0f|"),
    #    CONF_BATTERY_POWER_DIRECTION: ([0, 1, 2], "GS.....")
    #    CONF_CHARGER_SOURCE_PRIORITY: ([0, 1, 2, 3], "PCP%02.0f"),
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
            if (CONF_POSSIBLE_VALUES) in conf:
                cg.add(var.set_possible_values(conf[CONF_POSSIBLE_VALUES]))


@automation.register_action(
    "output.pipsolar.set_level",
    SetOutputAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(CONF_ID),
            cv.Required(CONF_VALUE): cv.templatable(cv.positive_float),
        }
    ),
)
def output_pipsolar_set_level_to_code(config, action_id, template_arg, args):
    paren = yield cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = yield cg.templatable(config[CONF_VALUE], args, float)
    cg.add(var.set_level(template_))
    yield var
