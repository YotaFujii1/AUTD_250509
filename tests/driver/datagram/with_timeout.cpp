#include <gtest/gtest.h>

#include <autd3/driver/datagram/with_timeout.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(DriverDatagram, WithTimeout) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());

  autd.send(autd3::gain::Null().with_timeout(std::nullopt));
  ASSERT_EQ(autd.link().last_timeout(), std::nullopt);

  autd.send(autd3::gain::Null().with_timeout(std::chrono::milliseconds(10)));
  ASSERT_EQ(autd.link().last_timeout(), std::chrono::milliseconds(10));

  autd.send((autd3::gain::Null(), autd3::modulation::Static()).with_timeout(std::chrono::milliseconds(20)));
  ASSERT_EQ(autd.link().last_timeout(), std::chrono::milliseconds(20));
}
