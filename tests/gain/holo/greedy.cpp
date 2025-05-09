#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/gain/holo.hpp>
#include <autd3/gain/holo/greedy.hpp>
#include <autd3/link/audit.hpp>

TEST(GainHolo, Greedy) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());

  std::vector<float> p{-30, 30};
  auto g = autd3::gain::holo::Greedy(p | std::ranges::views::transform([&](auto x) {
                                       autd3::driver::Vector3 focus = autd.center() + autd3::driver::Vector3(x, 0, 150);
                                       return std::make_pair(focus, 5e3 * autd3::gain::holo::Pa);
                                     }))
               .with_phase_div(16);
  g.with_constraint(autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::EmitIntensity(0x80)));

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

TEST(GainHolo, GreedyDefault) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());
  std::vector<std::pair<autd3::driver::Vector3, autd3::gain::holo::Amplitude>> foci;
  const auto g = autd3::gain::holo::Greedy(foci);
  ASSERT_TRUE(autd3::native_methods::AUTDGainGreedyIsDefault(g.constraint(), g.phase_div()));
}
