#include <gtest/gtest.h>

#include <autd3/modulation/square.hpp>

#include "utils.hpp"

TEST(Modulation, Square) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Square(200 * autd3::driver::Hz).with_low(32).with_high(85).with_duty(0.1f));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{85, 85, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }

  autd.send(autd3::modulation::Square(150 * autd3::driver::Hz).with_sampling_config(autd3::driver::SamplingConfig(20)));

  for (auto& dev : autd) ASSERT_EQ(20, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
}

TEST(Modulation, SquareWithMode) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Square::nearest(150.f * autd3::driver::Hz));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
  }

  ASSERT_THROW(autd.send(autd3::modulation::Square(100.1f * autd3::driver::Hz)), autd3::AUTDException);
  autd.send(autd3::modulation::Square::nearest(100.1f * autd3::driver::Hz));
}

TEST(Modulation, SquareDefault) {
  const auto m = autd3::modulation::Square(150.0f * autd3::driver::Hz);
  ASSERT_TRUE(AUTDModulationSquareIsDefault(m.sampling_config(), m.low(), m.high(), m.duty(), m.loop_behavior()));
}
