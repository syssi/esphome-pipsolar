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
  using Pipsolar::handle_qpgs0_;
  using Pipsolar::handle_q1_;
  using Pipsolar::handle_qbms_;
  using Pipsolar::handle_qet_;
  using Pipsolar::handle_qlt_;
  using Pipsolar::handle_qt_;
};

}  // namespace esphome::pip8048::testing
