#include <gtest/gtest.h>

#include <autd3/driver/datagram/modulation/radiation_pressure.hpp>
#include <autd3/modulation/sine.hpp>

#include "utils.hpp"

TEST(DriverDatagramModulation, RadiationPressure) {
  auto autd = create_controller();

  {
    autd.send(autd3::modulation::Sine(150 * autd3::driver::Hz).with_radiation_pressure());

    for (auto& dev : autd) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{
          181, 200, 217, 231, 243, 250, 254, 255, 252, 245, 236, 222, 206, 188, 166, 142, 116, 89, 60, 32, 0,  32, 60, 89,  116, 142, 166,
          188, 206, 222, 236, 245, 252, 255, 254, 250, 243, 231, 217, 200, 181, 158, 134, 107, 78, 50, 23, 0,  39, 70, 97,  125, 150, 173,
          194, 212, 227, 239, 248, 253, 255, 253, 248, 239, 227, 212, 194, 173, 150, 125, 97,  70, 39, 0,  23, 50, 78, 107, 134, 158,
      };
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }

  {
    autd3::modulation::Sine m(150 * autd3::driver::Hz);
    autd.send(m.with_radiation_pressure());

    for (auto& dev : autd) {
      auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
      std::vector<uint8_t> mod_expect{
          181, 200, 217, 231, 243, 250, 254, 255, 252, 245, 236, 222, 206, 188, 166, 142, 116, 89, 60, 32, 0,  32, 60, 89,  116, 142, 166,
          188, 206, 222, 236, 245, 252, 255, 254, 250, 243, 231, 217, 200, 181, 158, 134, 107, 78, 50, 23, 0,  39, 70, 97,  125, 150, 173,
          194, 212, 227, 239, 248, 253, 255, 253, 248, 239, 227, 212, 194, 173, 150, 125, 97,  70, 39, 0,  23, 50, 78, 107, 134, 158,
      };
      ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
      ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
    }
  }
}
