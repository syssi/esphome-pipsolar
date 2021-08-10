# esphome-pipsolar

ESPHome component to monitor and control a pipsolar inverter via RS232

:mega: This is a short-lived fork of https://github.com/esphome/esphome/pull/1664. Please use the official component as soon it's merged! :mega:

Kudos to [@andreashergert1984](https://github.com/andreashergert1984) for the great work!

## Supported devices

* PIP4048 compatible PV Inverter


## Requirements

* [ESPHome 1.18.0 or higher](https://github.com/esphome/esphome/releases).
* One half of an ethernet cable with RJ45 connector
* RS232-to-TTL module (`MAX3232CSE` f.e.)
* Generic ESP32 or ESP8266 board

## Schematics

```
               RS232                       TTL
┌─────────┐              ┌──────────┐                ┌─────────┐
│         │              │          │<----- RX ----->│         │
│         │<---- TX ---->│  RS232   │<----- TX ----->│ ESP32/  │
│   PIP   │<---- RX ---->│  to TTL  │<----- GND ---->│ ESP8266 │
│         │<---- GND --->│  module  │<-- 3.3V VCC -->│         │
│         │              └──────────┘                │         │
│         │              ┌──────────┐                │         │
│         │----- 12V --> │ Buck     │--- 3.3V VCC -->│         │
└─────────┘              │ Converter│                │         │
                         └──────────┘                └─────────┘

```

### PIP RJ45 jack

| Pin     | Purpose      | RS232-to-TTL pin  | Color T-568B |
| :-----: | :----------- | :---------------- | :------------|
|    1    | TX           | TX &#8592;        | White-Orange |
|    2    | RX           | RX &#8594;        | Orange       |
|    3    |              |                   |              |
|    4    | VCC 12V (?)  | -                 | Blue         |
|    5    |              |                   |              |
|    6    |              |                   |              |
|    7    | VCC 12V      | -                 | White-Brown  |
|    8    | GND          | GND               | Brown        |

Please be aware of the different RJ45 pinout colors ([T-568A vs. T-568B](images/rj45-colors-t568a-vs-t568.png)).

The inverter provides +12V on pin 4 or 7 depending on the model. You can use a cheap DC-DC converter to power the ESP with 3.3V.

The [source for the pinout is here](docs/HS_MS_MSX%20RS232%20Protocol.pdf).

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-pipsolar@main
```

or just use the `esp32-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-pipsolar.git
cd esphome-pipsolar

# Create a secret.yaml containing some setup specific secrets
cat > secret.yaml <<EOF
mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD

wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
# If you want to use a esp8266 the `esphome:` and `uart:` section must be adapted.
esphome run esp32-example.yaml

```

## References

* https://github.com/esphome/esphome/pull/1664
* https://github.com/esphome/esphome-docs/pull/1084/files
* https://github.com/andreashergert1984/esphome/tree/feature_pipsolar_anh
* https://github.com/jblance/mpp-solar/tree/master/docs/protocols
