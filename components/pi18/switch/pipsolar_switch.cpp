#include "pipsolar_switch.h"
#include "esphome/core/log.h"

namespace esphome::pi18 {

static const char *const TAG = "pi18.switch";

void PipsolarSwitch::dump_config() { LOG_SWITCH("", "PI18 Switch", this); }
void PipsolarSwitch::write_state(bool state) {
  const char *command = state ? this->on_command_ : this->off_command_;
  if (command != nullptr) {
    this->parent_->queue_command(command);
  }
}

}  // namespace esphome::pi18
