#include <gtest/gtest.h>

#include <autd3/controller/controller.hpp>
#include <autd3/controller/timer.hpp>

#include "utils.hpp"

TEST(Controller, Timer) {
  {
    auto autd = autd3::controller::ControllerBuilder({
                                                         autd3::driver::AUTD3(autd3::driver::Vector3::Zero()),
                                                     })
                    .with_timer_strategy(autd3::controller::timer::TimerStrategy::Std(autd3::controller::timer::StdSleeper{}))
                    .open(autd3::link::Audit::builder());
    autd.close();
  }
  {
    auto autd = autd3::controller::ControllerBuilder(
                    std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                    .with_timer_strategy(autd3::controller::timer::TimerStrategy::Spin(autd3::controller::timer::SpinSleeper()))
                    .open(autd3::link::Audit::builder());
    autd.close();
  }
  {
    auto autd = autd3::controller::ControllerBuilder(
                    std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                    .with_timer_strategy(autd3::controller::timer::TimerStrategy::Async(autd3::controller::timer::AsyncSleeper{}))
                    .open(autd3::link::Audit::builder());
    autd.close();
  }
#ifdef WIN32
  {
    auto autd = autd3::controller::ControllerBuilder(
                    std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero()), autd3::driver::AUTD3(autd3::driver::Vector3::Zero())})
                    .with_timer_strategy(autd3::controller::timer::TimerStrategy::Waitable(autd3::controller::timer::WaitableSleeper()))
                    .open(autd3::link::Audit::builder());
    autd.close();
  }
#endif
}
