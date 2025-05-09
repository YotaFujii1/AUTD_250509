#include <gtest/gtest.h>

#include <autd3/driver/firmware_version.hpp>
#include <autd3/exception.hpp>
#include <ranges>
#include <sstream>

#include "utils.hpp"

TEST(Driver, FirmwareVersion) {
  auto autd = create_controller();

  ASSERT_EQ("v10.0.1", autd3::driver::FirmwareVersion::latest_version());

  {
    const auto infos = autd.firmware_version();
    std::ranges::for_each(std::ranges::views::iota(0) | std::ranges::views::take(infos.size()), [&](auto i) {
      std::stringstream ss;
      ss << i;
      ss << ": CPU = v10.0.1, FPGA = v10.0.1 [Emulator]";
      ASSERT_EQ(ss.str(), infos[i].info());
      std::stringstream ssf;
      ssf << infos[i];
      ASSERT_EQ(ss.str(), ssf.str());
    });
  }

  {
    autd.link().break_down();
    ASSERT_THROW((void)autd.firmware_version(), autd3::AUTDException);
    autd.link().repair();
  }
}

#ifdef AUTD3_ASYNC_API
TEST(Driver, FirmwareVersionAsync) {
  auto autd = create_controller();

  {
    const auto infos = sync_wait(autd.firmware_version_async());
    std::ranges::for_each(std::ranges::views::iota(0) | std::ranges::views::take(infos.size()), [&](auto i) {
      std::stringstream ss;
      ss << i;
      ss << ": CPU = v10.0.1, FPGA = v10.0.1 [Emulator]";
      ASSERT_EQ(ss.str(), infos[i].info());
      std::stringstream ssf;
      ssf << infos[i];
      ASSERT_EQ(ss.str(), ssf.str());
    });
  }

  {
    autd.link().break_down();
    ASSERT_THROW(sync_wait(autd.firmware_version_async()), std::runtime_error);
    autd.link().repair();
  }
}
#endif
