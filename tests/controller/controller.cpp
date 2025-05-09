#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/gain/null.hpp>
#include <autd3/gain/uniform.hpp>
#include <autd3/modulation/sine.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

TEST(Controller, ControllerIsDefault) {
  auto autd = autd3::controller::ControllerBuilder(std::vector{autd3::driver::AUTD3(autd3::driver::Vector3::Zero())});

  ASSERT_TRUE(autd3::native_methods::AUTDControllerBuilderIsDefault(
      autd.default_parallel_threshold(), autd3::native_methods::to_duration(autd.default_timeout()),
      autd3::native_methods::to_duration(autd.send_interval()), autd3::native_methods::to_duration(autd.receive_interval()), autd.timer_strategy()));
}

TEST(Controller, ControllerClose) {
  {
    auto autd = create_controller();
    ASSERT_TRUE(autd.link().is_open());

    autd.close();
    autd.close();
  }

  {
    auto autd = create_controller();
    autd.link().break_down();
    ASSERT_THROW(autd.close(), autd3::AUTDException);
  }
}

#ifdef AUTD3_ASYNC_API
TEST(Controller, ControllerCloseAsync) {
  {
    auto autd = create_controller();
    ASSERT_TRUE(autd.link().is_open());

    sync_wait(autd.close_async());
    sync_wait(autd.close_async());
  }

  {
    auto autd = create_controller();
    autd.link().break_down();
    ASSERT_THROW(sync_wait(autd.close_async()), autd3::AUTDException);
  }
}
#endif

TEST(Controller, ControllerSendSingle) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.send(autd3::modulation::Static());
  for (auto& dev : autd) {
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.link().down();
  ASSERT_THROW(autd.send(autd3::modulation::Static()), autd3::AUTDException);
  autd.link().up();

  autd.link().break_down();
  ASSERT_THROW(autd.send(autd3::modulation::Static()), autd3::AUTDException);
  autd.link().repair();
}

#ifdef AUTD3_ASYNC_API
TEST(Controller, ControllerSendSingleAsync) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  sync_wait(autd.send_async(autd3::modulation::Static()));
  for (auto& dev : autd) {
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.link().down();
  ASSERT_THROW(sync_wait(autd.send_async(autd3::modulation::Static())), autd3::AUTDException);
  autd.link().up();

  autd.link().break_down();
  ASSERT_THROW(sync_wait(autd.send_async(autd3::modulation::Static())), autd3::AUTDException);
  autd.link().repair();
}
#endif

TEST(Controller, ControllerSendTuple) {
  auto autd = create_controller();

  for (auto& dev : autd) {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
  }

  autd.send((autd3::modulation::Static::with_intensity(0x80), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x81), autd3::driver::Phase(0x82))));
  for (auto& dev : autd) {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x81 && d.phase.value() == 0x82; }));
    auto m = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0x80; }));
  }
}

TEST(Controller, ControllerGroup) {
  auto autd = create_controller();

  autd.group([](auto& dev) -> std::optional<size_t> { return dev.idx(); })
      .set(0, (autd3::modulation::Static(), autd3::gain::Null()))
      .set(1, (autd3::modulation::Sine(150 * autd3::driver::Hz), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))))
      .send();

  {
    const auto m = autd.link().modulation(0, autd3::native_methods::Segment::S0);
    ASSERT_EQ(2, m.size());
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    const auto m = autd.link().modulation(1, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));
  }

  autd.group([](auto& dev) -> std::optional<size_t> { return dev.idx(); })
      .set(1, autd3::gain::Null())
      .set(0, (autd3::modulation::Sine(150 * autd3::driver::Hz), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))))
      .send();

  {
    const auto m = autd.link().modulation(0, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0; }));
  }
}

#ifdef AUTD3_ASYNC_API
TEST(Controller, ControllerGroupAsync) {
  auto autd = create_controller();

  sync_wait(autd.group([](auto& dev) -> std::optional<size_t> { return dev.idx(); })
                .set(0, (autd3::modulation::Static(), autd3::gain::Null()))
                .set(1, (autd3::modulation::Sine(150 * autd3::driver::Hz), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))))
                .send_async());

  {
    const auto m = autd.link().modulation(0, autd3::native_methods::Segment::S0);
    ASSERT_EQ(2, m.size());
    ASSERT_TRUE(std::ranges::all_of(m, [](auto d) { return d == 0xFF; }));
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    const auto m = autd.link().modulation(1, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));
  }

  sync_wait(autd.group([](auto& dev) -> std::optional<size_t> { return dev.idx(); })
                .set(1, autd3::gain::Null())
                .set(0, (autd3::modulation::Sine(150 * autd3::driver::Hz), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80))))
                .send_async());

  {
    const auto m = autd.link().modulation(0, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0; }));
  }
  {
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0; }));
  }
}
#endif

TEST(Controller, ControllerGroupCheckOnlyForEnabled) {
  auto autd = create_controller();
  autd[0].set_enable(false);

  std::vector check(autd.num_devices(), false);
  autd.group([&check](auto& dev) -> std::optional<int> {
        check[dev.idx()] = true;
        return 0;
      })
      .set(0,
           (autd3::modulation::Sine(150 * autd3::driver::Hz), autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90))))
      .send();

  ASSERT_FALSE(check[0]);
  ASSERT_TRUE(check[1]);

  {
    auto drives = autd.link().drives(0, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }
  {
    const auto m = autd.link().modulation(1, autd3::native_methods::Segment::S0);
    ASSERT_EQ(80, m.size());
    auto drives = autd.link().drives(1, autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }
}
