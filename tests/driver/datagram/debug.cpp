#include <gtest/gtest.h>

#include <autd3/driver/datagram/debug.hpp>

#include "utils.hpp"

TEST(DriverDatagram, DebugOutputIdx) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x00, 0x00, 0x00, 0x00};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::DebugSettings([](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> auto {
    switch (gpio) {
      case autd3::native_methods::GPIOOut::O0:
        return autd3::driver::DebugType::None;
      case autd3::native_methods::GPIOOut::O1:
        return autd3::driver::DebugType::BaseSignal;
      case autd3::native_methods::GPIOOut::O2:
        return autd3::driver::DebugType::Thermo;
      case autd3::native_methods::GPIOOut::O3:
        return autd3::driver::DebugType::ForceFan;
    }
    return autd3::driver::DebugType::None;  // LCOV_EXCL_LINE
  }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x00, 0x01, 0x02, 0x03};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::DebugSettings([](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> auto {
    switch (gpio) {
      case autd3::native_methods::GPIOOut::O0:
        return autd3::driver::DebugType::Sync;
      case autd3::native_methods::GPIOOut::O1:
        return autd3::driver::DebugType::ModSegment;
      case autd3::native_methods::GPIOOut::O2:
        return autd3::driver::DebugType::ModIdx(0x01);
      case autd3::native_methods::GPIOOut::O3:
        return autd3::driver::DebugType::StmSegment;
    }
    return autd3::driver::DebugType::None;  // LCOV_EXCL_LINE
  }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x10, 0x20, 0x21, 0x50};
    std::array<uint64_t, 4> value{0x0000, 0x0000, 0x0001, 0x0000};
    ASSERT_EQ(ty, autd.link().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link().debug_values(dev.idx()));
  }

  autd.send(autd3::driver::DebugSettings([](const autd3::driver::geometry::Device& dev, const autd3::native_methods::GPIOOut gpio) -> auto {
    switch (gpio) {
      case autd3::native_methods::GPIOOut::O0:
        return autd3::driver::DebugType::StmIdx(0x02);
      case autd3::native_methods::GPIOOut::O1:
        return autd3::driver::DebugType::IsStmMode;
      case autd3::native_methods::GPIOOut::O2:
        return autd3::driver::DebugType::PwmOut(&dev[3]);
      case autd3::native_methods::GPIOOut::O3:
        return autd3::driver::DebugType::Direct(true);
    }
    return autd3::driver::DebugType::None;  // LCOV_EXCL_LINE
  }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x51, 0x52, 0xE0, 0xF0};
    std::array<uint64_t, 4> value{0x0002, 0x0000, 0x0003, 0x0001};
    ASSERT_EQ(ty, autd.link().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link().debug_values(dev.idx()));
  }

  const auto sys_time = autd3::driver::DcSysTime::now();
  autd.send(autd3::driver::DebugSettings([sys_time](const autd3::driver::geometry::Device&, const autd3::native_methods::GPIOOut gpio) -> auto {
    switch (gpio) {
      case autd3::native_methods::GPIOOut::O0:
        return autd3::driver::DebugType::SysTimeEq(sys_time);
      case autd3::native_methods::GPIOOut::O1:
        return autd3::driver::DebugType::None;
      case autd3::native_methods::GPIOOut::O2:
        return autd3::driver::DebugType::None;
      case autd3::native_methods::GPIOOut::O3:
        return autd3::driver::DebugType::None;
    }
    return autd3::driver::DebugType::None;  // LCOV_EXCL_LINE
  }));
  for (auto& dev : autd) {
    std::array<uint8_t, 4> ty{0x60, 0x00, 0x00, 0x00};
    std::array<uint64_t, 4> value{(sys_time.sys_time() / 3125) << 5, 0x0000, 0x0000, 0x0000};
    ASSERT_EQ(ty, autd.link().debug_types(dev.idx()));
    ASSERT_EQ(value, autd.link().debug_values(dev.idx()));
  }
}
