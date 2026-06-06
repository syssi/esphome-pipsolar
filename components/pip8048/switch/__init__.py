import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import ICON_POWER

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA, pipsolar_ns

DEPENDENCIES = ["uart"]

CONF_OUTPUT_SOURCE_PRIORITY_UTILITY = "output_source_priority_utility"
CONF_OUTPUT_SOURCE_PRIORITY_SOLAR = "output_source_priority_solar"
CONF_OUTPUT_SOURCE_PRIORITY_BATTERY = "output_source_priority_battery"
CONF_OUTPUT_SOURCE_PRIORITY_HYBRID = "output_source_priority_hybrid"
CONF_INPUT_VOLTAGE_RANGE = "input_voltage_range"
CONF_PV_OK_CONDITION_FOR_PARALLEL = "pv_ok_condition_for_parallel"
CONF_PV_POWER_BALANCE = "pv_power_balance"
CONF_BUZZER = "buzzer"
CONF_OVERLOAD_BYPASS = "overload_bypass"
CONF_LCD_ESCAPE_TO_DEFAULT = "lcd_escape_to_default"
CONF_OVERLOAD_RESTART = "overload_restart"
CONF_OVER_TEMPERATURE_RESTART = "over_temperature_restart"
CONF_BACKLIGHT = "backlight"
CONF_ALARM_ON_PRIMARY_SOURCE_INTERRUPT = "alarm_on_primary_source_interrupt"
CONF_FAULT_CODE_RECORD = "fault_code_record"
CONF_POWER_SAVING = "power_saving"

TYPES = {
    CONF_OUTPUT_SOURCE_PRIORITY_UTILITY: ("POP00", None),
    CONF_OUTPUT_SOURCE_PRIORITY_SOLAR: ("POP01", None),
    CONF_OUTPUT_SOURCE_PRIORITY_BATTERY: ("POP02", None),
    CONF_OUTPUT_SOURCE_PRIORITY_HYBRID: ("POP03", None),
    CONF_INPUT_VOLTAGE_RANGE: ("PGR01", "PGR00"),
    CONF_PV_OK_CONDITION_FOR_PARALLEL: ("PPVOKC1", "PPVOKC0"),
    CONF_PV_POWER_BALANCE: ("PSPB1", "PSPB0"),
    CONF_BUZZER: ("PEa", "PDa"),
    CONF_OVERLOAD_BYPASS: ("PEb", "PDb"),
    CONF_LCD_ESCAPE_TO_DEFAULT: ("PEk", "PDk"),
    CONF_OVERLOAD_RESTART: ("PEu", "PDu"),
    CONF_OVER_TEMPERATURE_RESTART: ("PEv", "PDv"),
    CONF_BACKLIGHT: ("PEx", "PDx"),
    CONF_ALARM_ON_PRIMARY_SOURCE_INTERRUPT: ("PEy", "PDy"),
    CONF_FAULT_CODE_RECORD: ("PEz", "PDz"),
    CONF_POWER_SAVING: ("PEj", "PDj"),
}

PipsolarSwitch = pipsolar_ns.class_("PipsolarSwitch", switch.Switch, cg.Component)

PIPSWITCH_SCHEMA = switch.switch_schema(
    PipsolarSwitch, icon=ICON_POWER, block_inverted=True
).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): PIPSWITCH_SCHEMA for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type, (on, off) in TYPES.items():
        if type in config:
            conf = config[type]
            var = await switch.new_switch(conf)
            await cg.register_component(var, conf)
            cg.add(getattr(paren, f"set_{type}_switch")(var))
            cg.add(var.set_parent(paren))
            cg.add(var.set_on_command(on))
            if off is not None:
                cg.add(var.set_off_command(off))
