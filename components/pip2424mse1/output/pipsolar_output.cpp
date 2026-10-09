#include "pipsolar_output.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::pip2424mse1 {

ESPHOME_LOG_TAG(TAG, "pip2424mse1.output");

void PipsolarOutput::write_state(float state) {
  char tmp[16];
  snprintf(tmp, sizeof(tmp), this->set_command_, state);

  if (std::find(this->possible_values_.begin(), this->possible_values_.end(), state) != this->possible_values_.end()) {
    ESP_LOGD(TAG, "Will write: %s out of value %f / %02.0f", tmp, state, state);
    this->parent_->queue_command(std::string(tmp));
  } else {
    ESP_LOGD(TAG, "Will not write: %s as it is not in list of allowed values", tmp);
  }
}
}  // namespace esphome::pip2424mse1
