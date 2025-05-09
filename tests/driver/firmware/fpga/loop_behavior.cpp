#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/loop_behavior.hpp>
#include <autd3/exception.hpp>

TEST(DriverFirmwareFPGA, LoopBehaviorInfinite) {
  const auto [rep] = autd3::driver::LoopBehavior::Infinite;
  ASSERT_EQ(0xFFFF, rep);
}

TEST(DriverFirmwareFPGA, LoopBehaviorFinite) {
  const auto [rep] = autd3::driver::LoopBehavior::Finite(2);
  ASSERT_EQ(1, rep);

  ASSERT_THROW((void)autd3::driver::LoopBehavior::Finite(0), autd3::AUTDException);
}

TEST(DriverFirmwareFPGA, LoopBehaviorOnce) {
  const auto [rep] = autd3::driver::LoopBehavior::Once;
  ASSERT_EQ(0, rep);
}
