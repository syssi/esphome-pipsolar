#include "pipsolar_select.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::pip8048 {

ESPHOME_LOG_TAG(TAG, "pip8048.select");

void PipsolarSelect::dump_config() { LOG_SELECT(TAG, "Pipsolar Controller Select", this); }

void PipsolarSelect::control(const std::string &value) {
  ESP_LOGD(TAG, "got option: %s", value.c_str());
  const auto it = this->mapping_.find(value);
  if (it == this->mapping_.end()) {
    ESP_LOGD(TAG, "could not find option %s in mapping", value.c_str());
    return;
  }
  ESP_LOGD(TAG, "found mapped option %s for option %s", it->second.c_str(), value.c_str());
  this->parent_->queue_command(it->second);
  if (this->optimistic_)
    this->publish_state(value);
}

void PipsolarSelect::map_and_publish(const std::string &value) {
  ESP_LOGD(TAG, "got value: %s", value.c_str());
  const auto it = this->status_mapping_.find(value);
  if (it == this->status_mapping_.end()) {
    ESP_LOGD(TAG, "could not find option %s in mapping", value.c_str());
    return;
  }
  ESP_LOGD(TAG, "found mapped option %s for option %s", it->second.c_str(), value.c_str());
  this->publish_state(it->second);
}

}  // namespace esphome::pip8048
