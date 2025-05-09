#include <gtest/gtest.h>

#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(Modulation, Sine) {
  auto autd = create_controller();

  {
    const auto m = autd3::modulation::Sine(150 * autd3::driver::Hz)
                       .with_intensity(0x80)
                       .with_offset(0x40)
                       .with_phase(autd3::driver::pi / 2 * autd3::driver::rad);
    ASSERT_EQ(0x80, m.intensity());
    ASSERT_EQ(0x40, m.offset());
    ASSERT_EQ(autd3::driver::pi / 2 * autd3::driver::rad, m.phase());
    ASSERT_EQ(10, m.sampling_config().division());
    autd.send(m);

    for (auto& dev : autd) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{128, 126, 121, 112, 101, 88, 74, 58, 44, 30, 18, 9, 3, 0, 0, 4, 12, 22, 34, 49, 64, 78, 93,  105, 115, 123, 127,
                                      127, 124, 118, 109, 97,  83, 69, 53, 39, 26, 15, 6, 1, 0, 1, 6, 15, 26, 39, 53, 69, 83, 97,  109, 118, 124, 127,
                                      127, 123, 115, 105, 93,  78, 64, 49, 34, 22, 12, 4, 0, 0, 3, 9, 18, 30, 44, 58, 74, 88, 101, 112, 121, 126};
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }

  {
    const auto m = autd3::modulation::Sine(150 * autd3::driver::Hz)
                       .with_sampling_config(autd3::driver::SamplingConfig(20))
                       .with_loop_behavior(autd3::driver::LoopBehavior::Finite(10));
    ASSERT_EQ(20, m.sampling_config().division());
    autd.send(m);
    for (auto& dev : autd) ASSERT_EQ(20, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, SineWithMode) {
  auto autd = create_controller();

  autd.send(autd3::modulation::Sine::nearest(150.f * autd3::driver::Hz));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{128, 157, 185, 209, 230, 245, 253, 255, 250, 238, 220, 198, 171, 142,
                                    113, 84,  57,  35,  17,  5,   0,   2,   10,  25,  46,  70,  98};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
  }

  ASSERT_THROW(autd.send(autd3::modulation::Sine(100.1f * autd3::driver::Hz)), autd3::AUTDException);
  autd.send(autd3::modulation::Sine::nearest(100.1f * autd3::driver::Hz));
}

TEST(Modulation, SineDefault) {
  const autd3::modulation::Sine m(150.0f * autd3::driver::Hz);
  ASSERT_TRUE(AUTDModulationSineIsDefault(m.sampling_config(), m.intensity(), m.offset(), m.phase().radian(), m.clamp(), m.loop_behavior()));
}
