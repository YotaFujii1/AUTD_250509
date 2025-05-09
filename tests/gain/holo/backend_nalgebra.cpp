#include <gtest/gtest.h>

#include <autd3/gain/holo/backend_nalgebra.hpp>

TEST(GainHolo, BackendNewDelete) {
  const auto* backend = new autd3::gain::holo::NalgebraBackend;
  delete backend;
}