#include <gtest/gtest.h>

#include <autd3/driver/datagram/phase_corr.hpp>

#include "utils.hpp"

TEST(DriverDatagram, PhaseCorr) {
  auto autd = create_controller();

  autd.send(autd3::driver::PhaseCorrection(
      [&](const auto& dev) { return [&](const auto& tr) { return autd3::driver::Phase(static_cast<uint8_t>(dev.idx() + tr.idx())); }; }));

  for (const auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    for (const auto& tr : dev) {
      ASSERT_EQ(drives[tr.idx()].intensity.value(), 0x00);
      ASSERT_EQ(drives[tr.idx()].phase.value(), static_cast<uint8_t>(dev.idx() + tr.idx()));
    }
  }
}
