#pragma once
#include "esphome/components/pip8048/pipsolar.h"

namespace esphome::pip8048::testing {

class TestablePipsolar : public Pipsolar {
 public:
  using Pipsolar::handle_qbatcd_;
  using Pipsolar::handle_qflag_;
  using Pipsolar::handle_qmod_;
  using Pipsolar::handle_qpigs_;
  using Pipsolar::handle_qpigs2_;
  using Pipsolar::handle_qpiri_;
  using Pipsolar::handle_qpiws_;
};

}  // namespace esphome::pip8048::testing
