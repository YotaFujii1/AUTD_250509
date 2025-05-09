#include <gtest/gtest.h>

#include <autd3/driver/geometry/geometry.hpp>

#include "utils.hpp"

TEST(DriverGeomtry, AUTD3) {
  ASSERT_EQ(10.16f, autd3::driver::AUTD3::TRANS_SPACING);
  ASSERT_EQ(192.0f, autd3::driver::AUTD3::DEVICE_WIDTH);
  ASSERT_EQ(151.4f, autd3::driver::AUTD3::DEVICE_HEIGHT);
  ASSERT_EQ(18, autd3::driver::AUTD3::NUM_TRANS_IN_X);
  ASSERT_EQ(14, autd3::driver::AUTD3::NUM_TRANS_IN_Y);
  ASSERT_EQ(249, autd3::driver::AUTD3::NUM_TRANS_IN_UNIT);
}

TEST(DriverGeomtry, GeometryNumDevices) {
  auto autd = create_controller();
  ASSERT_EQ(autd.num_devices(), 2);
}

TEST(DriverGeomtry, GeometryNumTransducers) {
  auto autd = create_controller();
  ASSERT_EQ(autd.num_transducers(), 2 * 249);
}

TEST(DriverGeomtry, GeometryCenter) {
  auto autd = create_controller();
  ASSERT_NEAR_VECTOR3(autd.center(), autd3::driver::Vector3(86.625267028808594f, 66.71319580078125f, 0), 1e-6);
}

TEST(DriverGeomtry, GeometrySoundSpeed) {
  auto autd = create_controller();
  for (auto& dev : autd) ASSERT_EQ(dev.sound_speed(), 340e3);
  autd.set_sound_speed(350e3);
  for (auto& dev : autd) ASSERT_EQ(dev.sound_speed(), 350e3);
}

TEST(DriverGeomtry, GeometrySoundSpeedFromTemp) {
  auto autd = create_controller();
  autd.set_sound_speed_from_temp(15);
  for (auto& dev : autd) ASSERT_EQ(dev.sound_speed(), 340.29525e3f);
}
