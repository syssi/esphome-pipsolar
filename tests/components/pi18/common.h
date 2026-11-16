#pragma once
#include "esphome/components/pi18/pipsolar.h"

namespace esphome::pi18::testing {

class TestablePipsolar : public Pipsolar {
 public:
  using Pipsolar::handle_p005et_;
  using Pipsolar::handle_p006fws_;
  using Pipsolar::handle_p005gs_;
  using Pipsolar::handle_p006mod_;
  using Pipsolar::handle_p007flag_;
  using Pipsolar::handle_p007pgs0_;
  using Pipsolar::handle_p007piri_;
  using Pipsolar::handle_poll_error_;
  using Pipsolar::POLLING_COMMANDS_MAX;

  // Registered poll command n as text, or "" when that slot is unused
  std::string polling_command(size_t n) const {
    const auto &entry = this->enabled_polling_commands_[n];
    return entry.length == 0 ? std::string() : std::string((const char *) entry.command, entry.length);
  }
};

// switch_::Switch is abstract; tests only need publish_state()
class TestSwitch : public switch_::Switch {
 protected:
  void write_state(bool state) override {}
};

}  // namespace esphome::pi18::testing
