#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "autd3.hpp"
#include "autd3/gain/holo.hpp"

template <typename L>
inline void holo_test(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  autd3::modulation::Sine m(150 * autd3::Hz);  // 150Hz AM

  const autd3::Vector3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);

  std::cout << "Select Optimization Method (default is GSPAT)" << std::endl;

  std::cout << "[0]: GS" << std::endl;
  std::cout << "[1]: GSPAT" << std::endl;
  std::cout << "[2]: Naive" << std::endl;
  std::cout << "[3]: LM" << std::endl;
  std::cout << "[4]: Greedy" << std::endl;
  std::cout << "[Others]: GS-PAT" << std::endl;
  std::cout << "Choose number: ";

  std::string in;
  size_t idx;
  getline(std::cin, in);
  std::stringstream s(in);
  if (const auto is_empty = in == "\n"; !(s >> idx) || idx >= 5 || is_empty) idx = 1;

  auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
  auto amp = 5e3 * autd3::gain::holo::Pa;
  std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci = {
      {center + autd3::Vector3(30.0, 0.0, 0.0), amp},
      {center - autd3::Vector3(30.0, 0.0, 0.0), amp},
  };
  switch (idx) {
    case 0:
      autd.send((m, autd3::gain::holo::GS(backend, foci)));
      break;
    case 1:
      autd.send((m, autd3::gain::holo::GSPAT(backend, foci)));
      break;
    case 2:
      autd.send((m, autd3::gain::holo::Naive(backend, foci)));
      break;
    case 3:
      autd.send((m, autd3::gain::holo::LM(backend, foci)));
      break;
    case 4:
      autd.send((m, autd3::gain::holo::Greedy(foci)));
      break;
    default:
      break;
  }
}
