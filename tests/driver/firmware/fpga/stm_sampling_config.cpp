#include <gtest/gtest.h>

#include <autd3/driver/firmware/fpga/stm_sampling_config.hpp>

TEST(DriverFirmwareFPGA, STMSamplingConfigFreq) {
  const autd3::driver::STMSamplingConfig f(4000.0f * autd3::driver::Hz, 1);
  ASSERT_EQ(f.freq(), 4000.0f * autd3::driver::Hz);
  ASSERT_EQ(f.period(), std::chrono::microseconds(250));
  ASSERT_EQ(f.sampling_config(), autd3::driver::SamplingConfig(10));
}
