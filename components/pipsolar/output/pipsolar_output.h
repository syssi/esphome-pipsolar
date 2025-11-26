#pragma once

#include "../pipsolar.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/core/version.h"

namespace esphome {
namespace pipsolar {

class Pipsolar;

class PipsolarOutput : public output::FloatOutput {
 public:
  PipsolarOutput() {}
  void set_parent(Pipsolar *parent) { this->parent_ = parent; }
  void set_set_command(const std::string &command) { this->set_command_ = command; };
  void set_possible_values(std::vector<float> possible_values) { this->possible_values_ = std::move(possible_values); }
  void set_value(float value) { this->write_state(value); };

 protected:
  void write_state(float state) override;
  std::string set_command_;
  Pipsolar *parent_;
  std::vector<float> possible_values_;
};

template<typename... Ts> class SetOutputAction : public Action<Ts...> {
 public:
  SetOutputAction(PipsolarOutput *output) : output_(output) {}

  TEMPLATABLE_VALUE(float, level)
#if ESPHOME_VERSION_CODE >= VERSION_CODE(2025, 11, 0)
  void play(const Ts &...x) override { this->output_->set_value(this->level_.value(x...)); }
#else
  void play(Ts... x) override { this->output_->set_value(this->level_.value(x...)); }
#endif

 protected:
  PipsolarOutput *output_;
};

}  // namespace pipsolar
}  // namespace esphome
