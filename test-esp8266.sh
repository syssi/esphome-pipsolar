#!/bin/bash

if [[ $2 == tests/* ]]; then
  C="../components"
else
  C="components"
fi

esphome -s external_components_source $C ${1:-run} ${2:-esp8266-pip8048-example-faker.yaml}
