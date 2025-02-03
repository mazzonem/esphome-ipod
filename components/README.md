# Ipod ClickWheel custom sensor

This repository contains a custom components for [ESPHome](https://esphome.io/).

## 1. Usage

Use latest [ESPHome](https://esphome.io/) (at least v1.18.0)
with external components and add this to your `.yaml` definition:

```yaml
external_components:
  - source: github://mazzonem/esphome-ipod
```

## 2. Components

### 2.1. `click wheel`

```yaml
  sensor:
    - platform: click_wheel_sensor
      name: ClickWheel sensor
      pin_sda:
        number: GPIO05 #SDA
        mode: INPUT_PULLUP
      pin_scl:
        number: GPIO04 #SCL
        mode: INPUT_PULLUP
      deadband: 10 # Set the sensitivity of the wheel, the higher the number the less sensitive it is
      on_clockwise:
        - logger.log: "Turned Clockwise"
      on_anticlockwise:
        - logger.log: "Turned Anticlockwise"
      on_center:
        - logger.log: "Press Center"
      on_menu:
        - logger.log: "Press Menu"
      on_play:
        - logger.log: "Press Play Pause"
      on_forward:
        - logger.log: "Press Forward"
      on_backward:
        - logger.log: "Press Backward"
```