#include <gtest/gtest.h>

#include <autd3/modulation/fourier.hpp>
#include <ranges>

#include "utils.hpp"

TEST(Modulation, FourierExact) {
  auto autd = create_controller();

  std::vector f{50, 100, 150, 200, 250};
  const auto m =
      autd3::modulation::Fourier(f | std::ranges::views::transform([](const uint32_t x) { return autd3::modulation::Sine(x * autd3::driver::Hz); }));

  autd.send(m);

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        128, 157, 184, 206, 221, 228, 227, 219, 206, 189, 171, 154, 140, 130, 125, 124, 128, 134, 141, 148, 153, 156, 155, 152, 146, 139, 132,
        126, 121, 119, 120, 123, 128, 133, 137, 141, 142, 141, 138, 133, 128, 122, 117, 114, 113, 114, 118, 122, 128, 132, 135, 136, 134, 129,
        123, 116, 109, 103, 100, 99,  102, 107, 114, 121, 127, 131, 130, 125, 115, 101, 84,  66,  49,  36,  28,  27,  34,  49,  71,  98,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierExactFloat) {
  auto autd = create_controller();

  std::vector f{50.f, 100.f, 150.f, 200.f, 250.f};
  const auto m =
      autd3::modulation::Fourier(f | std::ranges::views::transform([](const float x) { return autd3::modulation::Sine(x * autd3::driver::Hz); }));

  autd.send(m);

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        128, 157, 184, 206, 221, 228, 227, 219, 206, 189, 171, 154, 140, 130, 125, 124, 128, 134, 141, 148, 153, 156, 155, 152, 146, 139, 132,
        126, 121, 119, 120, 123, 128, 133, 137, 141, 142, 141, 138, 133, 128, 122, 117, 114, 113, 114, 118, 122, 128, 132, 135, 136, 134, 129,
        123, 116, 109, 103, 100, 99,  102, 107, 114, 121, 127, 131, 130, 125, 115, 101, 84,  66,  49,  36,  28,  27,  34,  49,  71,  98,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierNearest) {
  auto autd = create_controller();

  std::vector f{50.f, 100.f};
  const auto m = autd3::modulation::Fourier(
      f | std::ranges::views::transform([](const float x) { return autd3::modulation::Sine::nearest(x * autd3::driver::Hz); }));
  autd.send(m);

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{
        128, 142, 157, 171, 185, 197, 208, 218, 226, 232, 236, 239, 240, 239, 236, 231, 226, 218, 210, 201, 191, 181, 171, 161, 151, 141, 133,
        125, 118, 113, 109, 106, 104, 104, 105, 107, 110, 113, 118, 123, 128, 132, 137, 142, 145, 148, 150, 151, 151, 149, 146, 142, 137, 130,
        122, 114, 104, 94,  84,  74,  64,  54,  45,  37,  29,  24,  19,  16,  15,  16,  19,  23,  29,  37,  47,  58,  70,  84,  98,  113,
    };
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, FourierInvalidMix) {
  auto autd = create_controller();

  const auto m = autd3::modulation::Fourier({autd3::modulation::Sine(50 * autd3::driver::Hz), autd3::modulation::Sine(100.0f * autd3::driver::Hz)});
  ASSERT_THROW(autd.send(m), std::bad_variant_access);
}
