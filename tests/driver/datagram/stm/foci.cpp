#include <gtest/gtest.h>

#include <autd3/driver/datagram/reads_fpga_state.hpp>
#include <autd3/driver/datagram/segment.hpp>
#include <autd3/driver/datagram/silencer.hpp>
#include <autd3/driver/datagram/stm/foci.hpp>
#include <autd3/driver/firmware/fpga/sampling_config.hpp>
#include <autd3/driver/firmware/fpga/transition_mode.hpp>
#include <ranges>

#include "utils.hpp"

using autd3::native_methods::Segment;

TEST(DriverDatagramSM, FociSTM) {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  {
    auto stm = autd3::driver::FociSTM(1.0f * autd3::driver::Hz, std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) {
                                                                  return autd3::driver::ControlPoints<1>{center};
                                                                }));
    ASSERT_EQ(1.0f * autd3::driver::Hz, stm.freq());
    ASSERT_EQ(std::chrono::seconds(1), stm.period());
    ASSERT_EQ(20000u, stm.sampling_config().division());
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    }

    for (const auto& dev : autd) ASSERT_EQ(20000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }

  {
    auto stm = autd3::driver::FociSTM::nearest(1.0f * autd3::driver::Hz, std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) {
                                                                           return autd3::driver::ControlPoints<1>{std::array{center}};
                                                                         }));
    autd.send(stm);
    for (const auto& dev : autd) {
      ASSERT_FALSE(autd.link().is_stm_gain_mode(dev.idx(), autd3::native_methods::Segment::S0));
    }

    for (const auto& dev : autd) ASSERT_EQ(20000u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }

  {
    auto stm = autd3::driver::FociSTM(autd3::driver::SamplingConfig(1),
                                      std::views::iota(0) | std::views::take(2) |
                                          std::views::transform([&](auto i) { return autd3::driver::ControlPoints<1>{center}.with_intensity(i); }));

    autd.send(stm);
    for (const auto& dev : autd) ASSERT_EQ(1u, autd.link().stm_freq_division(dev.idx(), autd3::native_methods::Segment::S0));

    for (const auto& dev : autd) {
      ASSERT_EQ(2u, autd.link().stm_cycle(dev.idx(), autd3::native_methods::Segment::S0));
      auto drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 0);
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() == 0x00; }));
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));

      drives = autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, 1);
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto d) { return d.intensity.value() == 0x01; }));
      ASSERT_TRUE(std::ranges::any_of(drives, [](auto p) { return p.phase.value() != 0; }));
    }
  }
}

TEST(DriverDatagramSM, FociSTMSegment) {
  auto autd = create_controller();

  autd.send(autd3::driver::ReadsFPGAState([](const auto&) { return true; }));

  auto infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd3::driver::Vector3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);
  auto stm = autd3::driver::FociSTM(1.0f * autd3::driver::Hz, std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) {
                                                                return autd3::driver::ControlPoints<1>{std::array{center}};
                                                              }));

  autd.send(stm);
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(stm.with_segment(Segment::S1, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::FociSTM(1.0f * autd3::driver::Hz, std::views::iota(0) | std::views::take(2) | std::views::transform([&](auto) {
                                                               return autd3::driver::ControlPoints<1>{std::array{center}};
                                                             }))
                .with_segment(Segment::S0, std::nullopt));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S1, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S1, autd.link().current_stm_segment(dev.idx()));
  }

  autd.send(autd3::driver::SwapSegment::FociSTM(Segment::S0, autd3::driver::TransitionMode::Immediate));
  infos = autd.fpga_state();
  for (auto& dev : autd) {
    ASSERT_EQ(std::nullopt, infos[dev.idx()].value().current_gain_segment());
    ASSERT_EQ(Segment::S0, infos[dev.idx()].value().current_stm_segment());
    ASSERT_EQ(Segment::S0, autd.link().current_stm_segment(dev.idx()));
  }
}

template <size_t N>
void test_foci_stm_n() {
  auto autd = create_controller();

  autd.send(autd3::driver::Silencer::disable());

  autd3::driver::Vector3 center = autd.center() + autd3::driver::Vector3(0, 0, 150);

  constexpr auto size = 2;
  std::array<autd3::driver::Vector3, N> points;
  points.fill(center);
  auto stm = autd3::driver::FociSTM(autd3::driver::SamplingConfig(1),
                                    std::views::iota(0) | std::views::take(size) |
                                        std::views::transform([&](auto i) { return autd3::driver::ControlPoints<N>{points}.with_intensity(i); }));

  autd.send(stm);
  for (const auto& dev : autd) {
    for (int i = 0; i < size; i++) {
      for (const auto& d : autd.link().drives(dev.idx(), autd3::native_methods::Segment::S0, i)) ASSERT_EQ(d.intensity.value(), i);
    }
  }
}

TEST(DriverDatagramSM, FociSTM_N1) { test_foci_stm_n<1>(); }
TEST(DriverDatagramSM, FociSTM_N2) { test_foci_stm_n<2>(); }
TEST(DriverDatagramSM, FociSTM_N3) { test_foci_stm_n<3>(); }
TEST(DriverDatagramSM, FociSTM_N4) { test_foci_stm_n<4>(); }
TEST(DriverDatagramSM, FociSTM_N5) { test_foci_stm_n<5>(); }
TEST(DriverDatagramSM, FociSTM_N6) { test_foci_stm_n<6>(); }
TEST(DriverDatagramSM, FociSTM_N7) { test_foci_stm_n<7>(); }
TEST(DriverDatagramSM, FociSTM_N8) { test_foci_stm_n<8>(); }
