#include "pipsolar_select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pip2424mse1 {

static const char *const TAG = "pip2424mse1.select";

void PipsolarSelect::dump_config() { LOG_SELECT(TAG, "Pipsolar Controller Select", this); }

void PipsolarSelect::control(const std::string &value) {
  ESP_LOGD(TAG, "got option: %s", value.c_str());
  if (this->mapping_.find(value) != this->mapping_.end()) {
    ESP_LOGD(TAG, "found mapped option %s for option %s", this->mapping_[value].c_str(), value.c_str());
    this->parent_->queue_command(this->mapping_[value]);
  } else {
    ESP_LOGD(TAG, "could not find option %s in mapping", value.c_str());
    return;
  }
  if (this->optimistic_)
    this->publish_state(value);
}

void PipsolarSelect::map_and_publish(std::string &value) {
  ESP_LOGD(TAG, "got value: %s", value.c_str());
  if (this->status_mapping_.find(value) != this->status_mapping_.end()) {
    ESP_LOGD(TAG, "found mapped option %s for option %s", this->status_mapping_[value].c_str(), value.c_str());
    this->publish_state(this->status_mapping_[value]);
  } else {
    ESP_LOGD(TAG, "could not find option %s in mapping", value.c_str());
  }
}

}  // namespace pip2424mse1
}  // namespace esphome
