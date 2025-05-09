#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/drive.hpp>

TEST(DriverFirmwareFPGA, Drive) {
  {
    const autd3::driver::Drive d(autd3::driver::Phase(0x90), autd3::driver::EmitIntensity(0x91));
    ASSERT_EQ(0x90, d.phase.value());
    ASSERT_EQ(0x91, d.intensity.value());
  }
  {
    const autd3::driver::Drive d(autd3::driver::EmitIntensity(0x91), autd3::driver::Phase(0x90));
    ASSERT_EQ(0x90, d.phase.value());
    ASSERT_EQ(0x91, d.intensity.value());
  }
  {
    const autd3::driver::Drive d(autd3::driver::Phase(0x90));
    ASSERT_EQ(0x90, d.phase.value());
    ASSERT_EQ(0xFF, d.intensity.value());
  }
  {
    const autd3::driver::Drive d(autd3::driver::EmitIntensity(0x90));
    ASSERT_EQ(0x00, d.phase.value());
    ASSERT_EQ(0x90, d.intensity.value());
  }
}
