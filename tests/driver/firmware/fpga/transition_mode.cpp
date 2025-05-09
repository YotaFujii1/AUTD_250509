#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/transition_mode.hpp>

TEST(DriverFirmwareFPGA, TransitionModeSyncIdx) {
  const auto [tag, value] = autd3::driver::TransitionMode::SyncIdx;
  ASSERT_EQ(tag, autd3::native_methods::TransitionModeTag::SyncIdx);
  ASSERT_EQ(value.null, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeSysTime) {
  const auto now = autd3::driver::DcSysTime::now();
  const auto [tag, value] = autd3::driver::TransitionMode::SysTime(now);
  ASSERT_EQ(tag, autd3::native_methods::TransitionModeTag::SysTime);
  ASSERT_EQ(value.sys_time.dc_sys_time, now.sys_time());
}

TEST(DriverFirmwareFPGA, TransitionModeGPIO) {
  const auto [tag, value] = autd3::driver::TransitionMode::GPIO(autd3::native_methods::GPIOIn::I1);
  ASSERT_EQ(tag, autd3::native_methods::TransitionModeTag::Gpio);
  ASSERT_EQ(value.gpio_in, autd3::native_methods::GPIOIn::I1);
}

TEST(DriverFirmwareFPGA, TransitionModeExt) {
  const auto [tag, value] = autd3::driver::TransitionMode::Ext;
  ASSERT_EQ(tag, autd3::native_methods::TransitionModeTag::Ext);
  ASSERT_EQ(value.null, 0);
}

TEST(DriverFirmwareFPGA, TransitionModeImmediate) {
  const auto [tag, value] = autd3::driver::TransitionMode::Immediate;
  ASSERT_EQ(tag, autd3::native_methods::TransitionModeTag::Immediate);
  ASSERT_EQ(value.null, 0);
}
