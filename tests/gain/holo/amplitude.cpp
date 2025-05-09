#include <gtest/gtest.h>

#include <autd3/gain/holo/amplitude.hpp>

TEST(GainHolo, AmplitudeDB) {
  const auto amp = 121.5 * autd3::gain::holo::dB;
  ASSERT_EQ(amp.spl(), 121.5);
  ASSERT_EQ(amp.pascal(), 23.7700348f);
}

TEST(GainHolo, AmplitudeSPL) {
  const auto amp = 23.7700348f * autd3::gain::holo::Pa;
  ASSERT_EQ(amp.spl(), 121.5);
  ASSERT_EQ(amp.pascal(), 23.7700348f);
}
