#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/phase.hpp>

TEST(DriverFirmwareFPGA, Phase) {
  const auto p = autd3::driver::Phase(std::numbers::pi_v<float> * autd3::driver::rad);
  ASSERT_EQ(128, p.value());
  ASSERT_NEAR(std::numbers::pi, p.radian(), 1e-6);
}
