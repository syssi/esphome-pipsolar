#include "pipsolar_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::pip8048 {

ESPHOME_LOG_TAG(TAG, "pip8048.switch");

void PipsolarSwitch::dump_config() { LOG_SWITCH("", "Pipsolar Switch", this); }
void PipsolarSwitch::write_state(bool state) {
  const char *command = state ? this->on_command_ : this->off_command_;
  if (command != nullptr) {
    this->parent_->queue_command(command);
  }
}

}  // namespace esphome::pip8048
