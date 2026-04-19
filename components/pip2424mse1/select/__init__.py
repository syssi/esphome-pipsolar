import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import CONF_ID

from .. import CONF_PIPSOLAR_ID, PIPSOLAR_COMPONENT_SCHEMA, pipsolar_ns
from ..const import CONF_OUTPUT_SOURCE_PRIORITY

DEPENDENCIES = ["uart"]

CONF_OPTIONSMAP = "optionsmap"
CONF_STATUSMAP = "statusmap"
CONF_CHARGING_DISCHARGING_CONTROL = "charging_discharging_control"

PipsolarSelect = pipsolar_ns.class_("PipsolarSelect", cg.Component, select.Select)


def ensure_option_map():
    def validator(value):
        cv.check_not_templatable(value)
        options_map_schema = cv.Schema(
            {cv.All(cv.string_strict): cv.All(cv.string_strict)}
        )
        value = options_map_schema(value)
        all_values = list(value.values())
        if len(all_values) != len(set(all_values)):
            raise cv.Invalid("Mapping values must be unique.")
        return value

    return validator


TYPES = {
    CONF_OUTPUT_SOURCE_PRIORITY: ("POP00", None),
    CONF_CHARGING_DISCHARGING_CONTROL: ("PBATCD111", None),
}

PIPSELECT_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(PipsolarSelect),
        cv.Optional(CONF_OPTIONSMAP): ensure_option_map(),
        cv.Optional(CONF_STATUSMAP): ensure_option_map(),
    }
).extend(select.select_schema(PipsolarSelect))

CONFIG_SCHEMA = PIPSOLAR_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): PIPSELECT_SCHEMA for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_PIPSOLAR_ID])

    for type in TYPES:
        if type in config:
            conf = config[type]
            options_map = conf[CONF_OPTIONSMAP]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await select.register_select(var, conf, options=list(options_map.keys()))
            cg.add(getattr(paren, f"set_{type}_select")(var))
            cg.add(var.set_parent(paren))
            for mappingkey in options_map:
                cg.add(var.add_mapping(mappingkey, options_map[mappingkey]))
            if CONF_STATUSMAP in conf:
                status_map = conf[CONF_STATUSMAP]
                for mappingkey in status_map:
                    cg.add(var.add_status_mapping(mappingkey, status_map[mappingkey]))
