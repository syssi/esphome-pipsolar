#pragma once
#include "esphome/components/pi18/pipsolar.h"

namespace esphome::pi18::testing {

class TestablePipsolar : public Pipsolar {
 public:
  using Pipsolar::handle_p005et_;
  using Pipsolar::handle_p005fws_;
  using Pipsolar::handle_p005gs_;
  using Pipsolar::handle_p006mod_;
  using Pipsolar::handle_p007flag_;
  using Pipsolar::handle_p007pgs0_;
  using Pipsolar::handle_p007piri_;
  using Pipsolar::handle_poll_error_;
};

// switch_::Switch is abstract; tests only need publish_state()
class TestSwitch : public switch_::Switch {
 protected:
  void write_state(bool state) override {}
};

}  // namespace esphome::pi18::testing
