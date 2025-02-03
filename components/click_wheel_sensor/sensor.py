import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins, automation
from esphome.components import sensor
from esphome.const import UNIT_EMPTY, ICON_EMPTY, CONF_TRIGGER_ID

click_wheel_sensor_ns = cg.esphome_ns.namespace("click_wheel_sensor")

CONF_PIN_SDA = "pin_sda"
CONF_PIN_SCL = "pin_scl"
CONF_DEADBAND = "deadband"
CONF_ON_CLOCKWISE = "on_clockwise"
CONF_ON_ANTICLOCKWISE = "on_anticlockwise"
CONF_ON_CENTER = "on_center"
CONF_ON_MENU = "on_menu"
CONF_ON_PLAY = "on_play"
CONF_ON_FORWARD = "on_forward"
CONF_ON_BACKWARD = "on_backward"

ClickWheelSensor = click_wheel_sensor_ns.class_("ClickWheelSensor", sensor.Sensor, cg.Component)

ClickWheelClockwiseTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelClockwiseTrigger", automation.Trigger
)
ClickWheelAnticlockwiseTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelAnticlockwiseTrigger", automation.Trigger
)
ClickWheelCenterTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelCenterTrigger", automation.Trigger
)
ClickWheelMenuTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelMenuTrigger", automation.Trigger
)
ClickWheelPlayTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelPlayTrigger", automation.Trigger
)
ClickWheelForwardTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelForwardTrigger", automation.Trigger
)
ClickWheelBackwardTrigger = click_wheel_sensor_ns.class_(
    "ClickWheelBackwardTrigger", automation.Trigger
)

CONFIG_SCHEMA = sensor.sensor_schema(
    ClickWheelSensor, unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=1
).extend(
         {
             cv.Required(CONF_PIN_SDA): cv.All(pins.internal_gpio_input_pin_schema),
             cv.Required(CONF_PIN_SCL): cv.All(pins.internal_gpio_input_pin_schema),
             cv.Optional(CONF_DEADBAND): cv.int_,
             cv.Optional(CONF_ON_CLOCKWISE): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelClockwiseTrigger),
                 }
             ),
             cv.Optional(CONF_ON_ANTICLOCKWISE): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelAnticlockwiseTrigger),
                 }
             ),
             cv.Optional(CONF_ON_CENTER): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelCenterTrigger),
                 }
             ),
             cv.Optional(CONF_ON_MENU): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelMenuTrigger),
                 }
             ),
             cv.Optional(CONF_ON_PLAY): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelPlayTrigger),
                 }
             ),
             cv.Optional(CONF_ON_FORWARD): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelForwardTrigger),
                 }
             ),
             cv.Optional(CONF_ON_BACKWARD): automation.validate_automation(
                 {
                     cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ClickWheelBackwardTrigger),
                 }
             ),
        }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    pin_sda = await cg.gpio_pin_expression(config[CONF_PIN_SDA])
    cg.add(var.set_pin_sda(pin_sda))
    pin_scl = await cg.gpio_pin_expression(config[CONF_PIN_SCL])
    cg.add(var.set_pin_scl(pin_scl))

    if CONF_DEADBAND in config:
        cg.add(var.set_deadband_value(config[CONF_DEADBAND]))

    for conf in config.get(CONF_ON_CLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_ANTICLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_CENTER, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_MENU, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_PLAY, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_FORWARD, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_BACKWARD, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)