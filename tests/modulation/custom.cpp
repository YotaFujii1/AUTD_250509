#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/modulation/custom.hpp>

#include "utils.hpp"

TEST(DriverDatagramModulation, Custom) {
  auto autd = create_controller();

  std::vector buffer(10, std::numeric_limits<uint8_t>::min());
  buffer[0] = std::numeric_limits<uint8_t>::max();

  autd.send(autd3::modulation::Custom(buffer, autd3::driver::SamplingConfig(5120)));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{255, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(5120, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, CustomResampler) {
  auto autd = create_controller();

  {
    autd.send(
        autd3::modulation::Custom({127, 255, 127, 0}, 2.0f * autd3::driver::kHz, 4u * autd3::driver::kHz, autd3::modulation::SincInterpolation()));

    for (auto& dev : autd) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{127, 217, 255, 217, 127, 37, 0, 37};
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }

  {
    autd.send(autd3::modulation::Custom({127, 255, 127, 0}, 2.0f * autd3::driver::kHz, 4u * autd3::driver::kHz,
                                        autd3::modulation::SincInterpolation(autd3::modulation::Rectangular{32})));

    for (auto& dev : autd) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{127, 217, 255, 223, 127, 42, 0, 37};
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }
}
