#include <gtest/gtest.h>

#include <autd3/modulation/audio_file/wav.hpp>

#include "utils.hpp"

TEST(Modulation, Wav) {
  auto autd = create_controller();

  const std::filesystem::path path = std::filesystem::path(AUTD3_RESOURCE_PATH).append("sin150.wav");
  autd.send(autd3::modulation::audio_file::Wav(path));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{128, 157, 185, 210, 230, 245, 253, 254, 248, 236, 217, 194, 167, 137, 109, 80,  54,  32,  15,  5,
                                    1,   5,   15,  32,  54,  80,  109, 137, 167, 194, 217, 236, 248, 254, 253, 245, 230, 210, 185, 157,
                                    128, 99,  71,  46,  26,  11,  3,   2,   8,   20,  39,  62,  89,  119, 147, 176, 202, 224, 241, 251,
                                    255, 251, 241, 224, 202, 176, 147, 119, 89,  62,  39,  20,  8,   2,   3,   11,  26,  46,  71,  99};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}

TEST(Modulation, WavResampler) {
  auto autd = create_controller();

  const std::filesystem::path path = std::filesystem::path(AUTD3_TEST_AUDIOFILE_RESOURCE_PATH).append("custom.wav");
  autd.send(autd3::modulation::audio_file::Wav(path, 4u * autd3::driver::kHz, autd3::modulation::SincInterpolation()));

  for (auto& dev : autd) {
    auto mod = autd.link().modulation(dev.idx(), autd3::native_methods::Segment::S0);
    std::vector<uint8_t> mod_expect{127, 217, 255, 217, 127, 37, 0, 37};
    ASSERT_TRUE(std::ranges::equal(mod, mod_expect, [](const auto& l, const auto& r) { return l == r; }));
    ASSERT_EQ(10, autd.link().modulation_freq_division(dev.idx(), autd3::native_methods::Segment::S0));
  }
}
