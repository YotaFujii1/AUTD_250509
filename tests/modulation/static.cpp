#include <gtest/gtest.h>

#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(Modulation, Static) {
  auto autd = create_controller();

  const auto m = autd3::modulation::Static::with_intensity(32);
  autd.send(m);
  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{32, 32};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(0xFFFF, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, StaticDefault) {
  const auto m = autd3::modulation::Static();
  ASSERT_TRUE(autd3::native_methods::AUTDModulationStaticIsDefault(m.intensity()));
}
