#include <gtest/gtest.h>

#include <autd3/controller/builder.hpp>
#include <autd3/controller/controller.hpp>
#include <autd3/driver/autd3_device.hpp>
#include <autd3/gain/holo.hpp>
#include <autd3/gain/holo/lm.hpp>
#include <autd3/link/audit.hpp>

TEST(GainHolo, LM) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  std::vector<float> p{-30, 30};
  const auto g = autd3::gain::holo::LM(std::move(backend), p | std::ranges::views::transform([&](auto x) {
                                                             autd3::driver::Vector3 p = autd.center() + autd3::driver::Vector3(x, 0, 150);
                                                             return std::make_pair(p, 5e3 * autd3::gain::holo::Pa);
                                                           }))
                     .with_eps1(1e-3f)
                     .with_eps2(1e-3f)
                     .with_tau(1e-3f)
                     .with_k_max(5)
                     .with_initial(std::vector{1.0f})
                     .with_constraint(autd3::gain::holo::EmissionConstraint::Uniform(autd3::driver::EmitIntensity(0x80)));

  autd.send(g);

  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80; }));
    ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.phase.value() != 0; }));
  }
}

TEST(GainHolo, LMDefault) {
  auto autd = autd3::controller::ControllerBuilder({autd3::driver::AUTD3(autd3::driver::Vector3::Zero())}).open(autd3::link::Audit::builder());
  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  std::vector<std::pair<autd3::driver::Vector3, autd3::gain::holo::Amplitude>> foci;
  const auto g = autd3::gain::holo::LM(std::move(backend), foci);
  ASSERT_TRUE(autd3::native_methods::AUTDGainLMIsDefault(g.constraint(), g.eps1(), g.eps2(), g.tau(), g.k_max(), g.initial().data(),
                                                         static_cast<uint32_t>(g.initial().size())));
}
