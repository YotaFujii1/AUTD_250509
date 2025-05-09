#include <gtest/gtest.h>

#include <autd3/driver/ethercat/dc_sys_time.hpp>

TEST(DriverEtherCAT, DcSysTime) {
  const auto now = autd3::driver::DcSysTime::now();
  ASSERT_TRUE(now.sys_time() > 0);
}
