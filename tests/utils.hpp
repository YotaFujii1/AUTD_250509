#pragma once

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/link/audit.hpp>

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

static inline autd3::controller::Controller<autd3::link::Audit> create_controller() {
  return autd3::controller::ControllerBuilder(
             std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
      .with_default_parallel_threshold(4)
      .with_default_timeout(std::chrono::milliseconds(20))
      .with_send_interval(std::chrono::milliseconds(1))
      .with_receive_interval(std::chrono::milliseconds(1))
      .open(autd3::link::Audit::builder());
}

#define ASSERT_NEAR_VECTOR3(val1, val2, abs_error) \
  do {                                             \
    ASSERT_NEAR(val1.x(), val2.x(), abs_error);    \
    ASSERT_NEAR(val1.y(), val2.y(), abs_error);    \
    ASSERT_NEAR(val1.z(), val2.z(), abs_error);    \
  } while (false)
