#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <autd3/gain/uniform.hpp>
#include <autd3/modulation/static.hpp>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagram, SwapSegmentGain) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
    auto drives = autd.link().drives(dev.idx(), Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0 && d.phase.value() == 0; }));
  }

  autd.send(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x80), autd3::driver::Phase(0x90)));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
    auto drives = autd.link().drives(dev.idx(), Segment::S0, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x80 && d.phase.value() == 0x90; }));
  }

  auto g = autd3::gain::Uniform(autd3::driver::EmitIntensity(0x81), autd3::driver::Phase(0x91));
  autd.send(g.with_segment(Segment::S1, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
    auto drives = autd.link().drives(dev.idx(), Segment::S1, 0);
    ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x81 && d.phase.value() == 0x91; }));
  }

  autd.send(autd3::gain::Uniform(autd3::driver::EmitIntensity(0x82), autd3::driver::Phase(0x92)).with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
    {
      auto drives = autd.link().drives(dev.idx(), Segment::S0, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x82 && d.phase.value() == 0x92; }));
    }
    {
      auto drives = autd.link().drives(dev.idx(), Segment::S1, 0);
      ASSERT_TRUE(std::ranges::all_of(drives, [](auto d) { return d.intensity.value() == 0x81 && d.phase.value() == 0x91; }));
    }
  }

  autd.send(autd3::driver::SwapSegment::Gain(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }
}

TEST(DriverDatagram, SwapSegmentModulation) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_mod_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_mod_segment(dev.idx()));
    ASSERT_TRUE(std::ranges::all_of(autd.link().modulation(dev.idx(), Segment::S0), [](auto d) { return d == 0xFF; }));
  }

  autd.send(autd3::modulation::Static::with_intensity(0x80));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_mod_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_mod_segment(dev.idx()));
    ASSERT_TRUE(std::ranges::all_of(autd.link().modulation(dev.idx(), Segment::S0), [](auto d) { return d == 0x80; }));
  }

  auto m = autd3::modulation::Static::with_intensity(0x81);
  autd.send(m.with_segment(Segment::S1, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_mod_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_mod_segment(dev.idx()));
    ASSERT_TRUE(std::ranges::all_of(autd.link().modulation(dev.idx(), Segment::S1), [](auto d) { return d == 0x81; }));
  }

  autd.send(autd3::modulation::Static::with_intensity(0x82).with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_mod_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_mod_segment(dev.idx()));
    ASSERT_TRUE(std::ranges::all_of(autd.link().modulation(dev.idx(), Segment::S0), [](auto d) { return d == 0x82; }));
    ASSERT_TRUE(std::ranges::all_of(autd.link().modulation(dev.idx(), Segment::S1), [](auto d) { return d == 0x81; }));
  }

  autd.send(autd3::driver::SwapSegment::Modulation(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_mod_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_mod_segment(dev.idx()));
  }
}
