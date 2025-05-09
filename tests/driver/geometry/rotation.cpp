#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/driver/geometry/rotation.hpp>
#include <autd3/link/audit.hpp>
#include <numbers>

#include "utils.hpp"

TEST(DriverGeomtry, Angle) {
  ASSERT_NEAR((90.0 * autd3::driver::deg).radian(), std::numbers::pi / 2, 1e-6);
  ASSERT_NEAR((std::numbers::pi_v<float> / 2 * autd3::driver::rad).radian(), std::numbers::pi / 2, 1e-6);
}

namespace {
autd3::controller::Controller<autd3::link::Audit> open_rotation(const autd3::driver::Quaternion& q) {
  return autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero()).with_rotation(q)})
      .open(autd3::link::Audit::builder());
}
}  // namespace

TEST(DriverGeomtry, WithRotationXYZ) {
  using autd3::driver::deg;
  using autd3::driver::Vector3;
  using autd3::driver::geometry::EulerAngles;

  {
    const auto autd = open_rotation(EulerAngles::XYZ(90.0 * deg, 0.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(-Vector3::UnitY(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::XYZ(0.0 * deg, 90.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(-Vector3::UnitZ(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::XYZ(0.0 * deg, 0.0 * deg, 90.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(-Vector3::UnitX(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::XYZ(0.0 * deg, 90.0 * deg, 90.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::XYZ(90.0 * deg, 90.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].axial_direction(), 1e-6);
  }
}

TEST(DriverGeomtry, WithRotationZYZ) {
  using autd3::driver::deg;
  using autd3::driver::Vector3;
  using autd3::driver::geometry::EulerAngles;

  {
    const auto autd = open_rotation(EulerAngles::ZYZ(90.0 * deg, 0.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(-Vector3::UnitX(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::ZYZ(0.0 * deg, 90.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(-Vector3::UnitZ(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::ZYZ(0.0 * deg, 0.0 * deg, 90.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(-Vector3::UnitX(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::ZYZ(0.0 * deg, 90.0 * deg, 90.0 * deg));
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitZ(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitX(), autd[0].axial_direction(), 1e-6);
  }
  {
    const auto autd = open_rotation(EulerAngles::ZYZ(90.0 * deg, 90.0 * deg, 0.0 * deg));
    ASSERT_NEAR_VECTOR3(-Vector3::UnitZ(), autd[0].x_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(-Vector3::UnitX(), autd[0].y_direction(), 1e-6);
    ASSERT_NEAR_VECTOR3(Vector3::UnitY(), autd[0].axial_direction(), 1e-6);
  }
}