#include <gtest/gtest.h>

#include <autd3/gain/null.hpp>

#include "utils.hpp"

TEST(Gain, Null) {
  auto autd = create_controller();

  autd.send(autd3::gain::Null());

  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
}
