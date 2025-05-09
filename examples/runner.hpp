#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "autd3.hpp"
#include "tests/bessel.hpp"
#include "tests/custom.hpp"
#include "tests/flag.hpp"
#include "tests/focus.hpp"
#include "tests/group.hpp"
#include "tests/holo.hpp"
//#include "tests/mod_audio_file.hpp"
#include "tests/plane.hpp"
#include "tests/stm.hpp"

template <typename L>
inline void run(autd3::Controller<L>& autd) {
    using F = std::function<void(autd3::Controller<L>&)>;
    std::vector<std::pair<F, std::string>> tests = {
        std::pair(F{focus_test<L>}, "Single focus test"), std::pair(F{bessel_test<L>}, "Bessel beam test"),
        std::pair(F{plane_test<L>}, "Plane wave test"),   
        std::pair(F{focus_stm<L>}, "FociSTM test"),       std::pair(F{gain_stm<L>}, "GainSTM test"),
        std::pair(F{gain_stm_241227<L>}, "GainSTM test_241227"),std::pair(F{gain_stm_250106<L>}, "GainSTM test_UpdateRateMode"),
        std::pair(F{holo_test<L>}, "Multiple foci test"), std::pair(F{stm_silencer_comparison<L>},"GSTest250509"),
        std::pair(F{flag_test<L>}, "Flag test"),
        std::pair(F{custom<L>}, "Custom Gain test"),      std::pair(F{group_by_transducer_test<L>}, "Group (by Transducer) test") };


    if (autd.num_devices() >= 2) tests.emplace_back(F{ group_by_device_test<L> }, "Group (by Device) test");

    const auto firm_infos = autd.firmware_version();
    std::cout << "======== AUTD3 firmware information ========" << std::endl;
    std::copy(firm_infos.begin(), firm_infos.end(), std::ostream_iterator<autd3::FirmwareVersion>(std::cout, "\n"));
    std::cout << "============================================" << std::endl;

    while (true) {
        size_t i = 0;
        std::transform(tests.begin(), tests.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
            [&i](const auto& test) { return "[" + std::to_string(i++) + "]: " + test.second; });
        std::cout << "[Others]: finish." << std::endl;

        std::cout << "Choose number: ";
        std::string in;
        size_t idx;
        getline(std::cin, in);
        std::stringstream s(in);
        if (const auto is_empty = in == "\n"; !(s >> idx) || idx >= tests.size() || is_empty) break;

        tests[idx].first(autd);

        std::cout << "press any key to finish..." << std::endl;
        std::cin.ignore();

        std::cout << "finish." << std::endl;
        autd.send((autd3::gain::Null(), autd3::Silencer()));
    }

    autd.close();
}