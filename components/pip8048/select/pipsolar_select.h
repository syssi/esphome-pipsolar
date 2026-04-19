#pragma once

#include <map>
#include <utility>

#include "esphome/components/pip8048/pipsolar.h"
#include "esphome/components/select/select.h"
#include "esphome/core/component.h"

namespace esphome {
namespace pip8048 {
class Pipsolar;

class PipsolarSelect : public Component, public select::Select {
 public:
  void set_parent(Pipsolar *const parent) { this->parent_ = parent; }
  void set_optimistic(bool optimistic) { this->optimistic_ = optimistic; }
  void add_mapping(const std::string &key, std::string value) { this->mapping_[key] = std::move(value); }
  void add_status_mapping(const std::string &key, std::string value) { this->status_mapping_[key] = std::move(value); }
  void dump_config() override;
  void control(const std::string &value) override;
  void map_and_publish(std::string &value);

 protected:
  std::map<std::string, std::string> mapping_;
  std::map<std::string, std::string> status_mapping_;
  Pipsolar *parent_;
  bool optimistic_{false};
};

}  // namespace pip8048
}  // namespace esphome
