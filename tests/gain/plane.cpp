#include <gtest/gtest.h>

#include <autd3/gain/plane.hpp>

#include "utils.hpp"

TEST(Gain, Plane) {
  auto autd = create_controller();

  autd.send(autd3::gain::Plane(autd3::driver::Vector3::UnitZ())
                .with_intensity(autd3::driver::EmitIntensity(0x80))
                .with_phase_offset(autd3::driver::Phase(0x81)));

  for (auto& dev : autd) {
    auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x81; }));
  }
}

TEST(Gain, PlaneDefault) {
  auto autd = create_controller();
  const auto g = autd3::gain::Plane(autd3::driver::Vector3::UnitZ());
  ASSERT_TRUE(autd3::native_methods::AUTDGainPlanelIsDefault(g.intensity().value(), g.phase_offset().value()));
}
