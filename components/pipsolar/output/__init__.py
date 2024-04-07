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
# battery_type;  00 for AGM, 01 for Flooded battery, 02 for user define, 03 for Pylontech, 
#                04 for Shinheung, 05 for Weco, 06 for Soltaro, 07 for BAK, 08 for Lib, 09 for Lic
# current_max_ac_charging_current;
# output_source_priority; 00 / 01 / 02
# charger_source_priority;  For HS: 00 for utility first, 01 for solar first, 02 for solar and utility, 03 for only solar charging
#                           For MS/MSX: 00 for utility first, 01 for solar first, 03 for only solar charging
# battery_redischarge_voltage;  12V unit: 00.0V12V/12.3V/12.5V/12.8V/13V/13.3V/13.5V/13.8V/14V/14.3V/14.5
#                               24V unit: 00.0V/24V/24.5V/25V/25.5V/26V/26.5V/27V/27.5V/28V/28.5V/29V
#                               48V unit: 00.0V48V/49V/50V/51V/52V/53V/54V/55V/56V/57V/58V

CONF_BATTERY_BULK_VOLTAGE = "battery_bulk_voltage"
CONF_BATTERY_RECHARGE_VOLTAGE = "battery_recharge_voltage"
CONF_BATTERY_UNDER_VOLTAGE = "battery_under_voltage"
CONF_BATTERY_FLOAT_VOLTAGE = "battery_float_voltage"
CONF_BATTERY_TYPE = "battery_type"
CONF_CURRENT_MAX_AC_CHARGING_CURRENT = "current_max_ac_charging_current"
CONF_CURRENT_MAX_CHARGING_CURRENT = "current_max_charging_current"
CONF_OUTPUT_SOURCE_PRIORITY = "output_source_priority"
CONF_CHARGER_SOURCE_PRIORITY = "charger_source_priority"
CONF_BATTERY_REDISCHARGE_VOLTAGE = "battery_redischarge_voltage"

TYPES = {
    # 26. Bulk charging voltage (C.V voltage)
    CONF_BATTERY_BULK_VOLTAGE: (
        #[44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0],
        [48.0, 48.8, 49.6, 50.4, 51.2, 52.0, 52.8, 53.6, 54.4, 55.2, 56.0, 56.8, 57.0, 57.6, 58.4],
        #[3.0, 3.05, 3.1, 3.15, 3.2, 3.25, 3.3, 3.35, 3.4, 3.45, 3.5, 3.5, 3.55, 3.6, 3.65], # Single Cell Volt
        "PCVV%02.1f",
    ),
    CONF_BATTERY_RECHARGE_VOLTAGE: (
        [44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0],
        "PBCV%02.1f",
    ),
    # 29. Low DC cut-off voltage
    CONF_BATTERY_UNDER_VOLTAGE: (
        [42.0, 42.4, 43.2, 44, 44.8, 45.6, 46.4, 47.2, 48.0],
        #[2.625, 2.65, 2.7, 2.75, 2.8, 2.85, 2.9, 2.95, 3], # Single Cell Volt
        "PSDV%02.1f",
    ),
    # 27. Floating charging voltage
    CONF_BATTERY_FLOAT_VOLTAGE: (
        #[48.0, 49.0, 50.0, 51.0], 
        [48.0, 48.8, 49.6, 50.4, 51.2, 52.0, 52.8, 53.6, 54.4, 55.2, 56.0, 56.8, 57.0, 57.6, 58.4],
        #[3.0, 3.05, 3.1, 3.15, 3.2, 3.25, 3.3, 3.35, 3.4, 3.45, 3.5, 3.5, 3.55, 3.6, 3.65], # Single Cell Volt
        "PBFT%02.1f"
    ),
    # Setting battery type, 00 for AGM, 01 for Flooded battery, 02 for user define, 03 for Pylontech, 04 for Shinheung, 05 for Weco, 06 for Soltaro, 07 for BAK, 08 for Lib, 09 for Lic
    CONF_BATTERY_TYPE: ([0, 1, 2, 3, 4 ,5 ,6, 7, 8, 9], "PBT%02.0f"),
    CONF_CURRENT_MAX_AC_CHARGING_CURRENT: ([2, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120], "MUCHGC%04.0f"),
    #CONF_CURRENT_MAX_CHARGING_CURRENT: ([ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 ], "MCHGC%03.0f"),
    CONF_CURRENT_MAX_CHARGING_CURRENT: ([ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 ], "MNCHGC%03.0f"),
    CONF_OUTPUT_SOURCE_PRIORITY: ([0, 1, 2], "POP%02.0f"),
    CONF_CHARGER_SOURCE_PRIORITY: ([0, 1, 2, 3], "PCP%02.0f"),
    # 13. Setting voltage point back to battery mode when selecting SBU (SBU priority) in program 01.
    CONF_BATTERY_REDISCHARGE_VOLTAGE: (
        [0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58],
        "PBDV%02.1f",
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
