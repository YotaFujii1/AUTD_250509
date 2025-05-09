#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/emit_intensity.hpp>
#include <cmath>

TEST(DriverFirmwareFPGA, EmitIntensity) {
  for (int i = 0; i <= 0xFF; i++) {
    const auto intensity = autd3::driver::EmitIntensity(static_cast<uint8_t>(i));
    ASSERT_EQ(intensity.value(), i);
  }
}

TEST(DriverFirmwareFPGA, EmitIntensityDiv) {
  for (int i = 0; i <= 0xFF; i++) {
    const auto intensity = autd3::driver::EmitIntensity(static_cast<uint8_t>(i)) / 2;
    ASSERT_EQ(intensity.value(), i / 2);
  }
}
