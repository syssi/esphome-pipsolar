# esphome-pipsolar

![GitHub actions](https://github.com/syssi/esphome-pipsolar/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-pipsolar)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-pipsolar)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-pipsolar)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome example configuration to monitor and control a pipsolar inverter via RS232

Kudos to [@andreashergert1984](https://github.com/andreashergert1984) for the great work!

## Supported devices

* PIP4048 compatible PV Inverter


## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases).
* One half of an ethernet cable with RJ45 connector
* RS232-to-TTL module (`MAX3232CSE` f.e.)
* Generic ESP32 or ESP8266 board

## Schematics

<a href="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/001.jpg" target="_blank">
  <img src="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/001.jpg" height="172">
</a>

<a href="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/002.jpg" target="_blank">
  <img src="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/002.jpg" height="172">
</a>

<a href="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/004.jpg" target="_blank">
  <img src="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/004.jpg" height="172">
</a>

<a href="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/005.jpg" target="_blank">
  <img src="https://raw.githubusercontent.com/syssi/esphome-pipsolar/main/images/005.jpg" height="172">
</a>

```
               RS232                     UART-TTL
┌──────────┐              ┌──────────┐                ┌─────────┐
│          │              │          │<----- RX ----->│         │
│          │<---- TX ---->│  RS232   │<----- TX ----->│ ESP32/  │
│   PIP    │<---- RX ---->│  to TTL  │<----- GND ---->│ ESP8266 │
│          │<---- GND --->│  module  │<-- 3.3V VCC -->│         │<--- VCC
│          │              │          │                │         │<--- GND
└──────────┘              └──────────┘                └─────────┘
```

### RJ45 connector

| Pin     | Purpose      | MAX3232 pin       | Color T-568B |
| :-----: | :----------- | :---------------- | :------------|
|    1    | TX           | P13 (RIN1)        | White-Orange |
|    2    | RX           | P14 (DOUT1)       | Orange       |
|    3    |              |                   |              |
|    4    | VCC 12V      | -                 | Blue         |
|    5    |              |                   |              |
|    6    |              |                   |              |
|    7    |              |                   |              |
|    8    | GND          | P15 (GND)         | Brown        |

Please be aware of the different RJ45 pinout colors ([T-568A vs. T-568B](images/rj45-colors-t568a-vs-t568.png)).

The inverter provides +12V on pin 4 or 7 depending on the model. You can use a cheap DC-DC converter to power the ESP with 3.3V.

The [source for the pinout is here](docs/HS_MS_MSX%20RS232%20Protocol.pdf).

### MAX3232

| Pin          | Label        | ESPHome     | ESP8266 example  | ESP32 example |
| :----------- | :----------- | :---------- | :--------------- | :------------ |
| P11 (DIN1)   | TXD          | `tx_pin`    | `GPIO4`          | `GPIO16`      |
| P12 (ROUT1)  | RXD          | `rx_pin`    | `GPIO5`          | `GPIO17`      |
| P16 (VCC)    | VCC          |             |                  |               |
| P15 (GND)    | GND          |             |                  |               |

## Installation

Use the `esp32-example.yaml` / `esp8266-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-pipsolar.git
cd esphome-pipsolar

# Create a secret.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
# If you use a esp8266 run the esp8266-examle.yaml
esphome run esp32-example.yaml

```

Take a look at the [official documentation of the component](https://esphome.io/components/pipsolar.html) for additional details.

## Known issues

1. If you configure a lot of the possible sensors etc. it could be that you run out of memory (on esp32). If you configure nearly all sensors etc. you run in a stack-size issue. In this case you have to increase stack size: https://github.com/esphome/issues/issues/855

## Debugging

If this component doesn't work out of the box for your device please update your configuration to enable the debug output of the UART component and increase the log level to the see outgoing and incoming serial traffic:

```
logger:
  level: DEBUG
  # Don't write log messages to UART0 (GPIO1/GPIO3) if the inverter is connected to GPIO1/GPIO3
  baud_rate: 0

uart:
  id: uart_0
  baud_rate: 2400
  tx_pin: GPIO1
  rx_pin: GPIO3
  debug:
    direction: BOTH
    dummy_receiver: false
    after:
      delimiter: "\r"
    sequence:
      - lambda: UARTDebug::log_string(direction, bytes);
```

If you don't know the protocol of your inverter please use [this configuration](tests/esp8266-test-protocols.yaml) and try to identify to which request (`>>>`) your inverter responds (`<<<`). `NAK` is a negative response f.e. if the command isn't supported. If you have trouble to interpret the log please create an issue and provide your ESPHome log.

## References

* https://github.com/esphome/esphome/pull/1664
* https://github.com/esphome/esphome-docs/pull/1084/files
* https://github.com/andreashergert1984/esphome/tree/feature_pipsolar_anh
* https://github.com/jblance/mpp-solar/tree/master/docs/protocols
