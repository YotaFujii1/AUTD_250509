#include <gtest/gtest.h>

#include <algorithm>
#include <autd3/driver/datagram/pulse_width_encoder.hpp>
#include <iostream>
#include <random>

#include "utils.hpp"

TEST(DriverDatagram, PulseWidthEncoder) {
  auto autd = create_controller();

  std::vector<uint8_t> buf(256);

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::uniform_int_distribution<uint16_t> dist(0x00, 0xFF);
  std::ranges::generate(buf, [&] { return dist(engine) & 0xFF; });

  autd.send(autd3::driver::PulseWidthEncoder([&](const auto&) { return [&](auto i) { return buf[i]; }; }));

  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf, autd.link().pulse_width_encoder_table(1));

  std::vector<uint8_t> buf_default(256);
  std::ranges::for_each(std::views::iota(0, 256), [&](auto i) {
    buf_default[i] = static_cast<uint8_t>(std::round(std::asin(static_cast<double>(i) / 255.) / std::numbers::pi * 256.));
  });
  autd.send(autd3::driver::PulseWidthEncoder());
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(0));
  ASSERT_EQ(buf_default, autd.link().pulse_width_encoder_table(1));
}
