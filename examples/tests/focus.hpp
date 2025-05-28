#pragma once

#include "autd3.hpp"

template <typename L>
inline void focus_test(autd3::Controller<L>& autd) {
    auto silencer = autd3::Silencer::disable();
    autd.send(silencer);

    autd3::modulation::Sine m(150 * autd3::Hz);  // 150Hz AM

    const autd3::Vector3 center = autd.center() + autd3::Vector3(0.0, 0.0, 200.0);
    autd3::gain::Focus g(center);

    autd.send((m, g));

    int intensity = 250; // 初期値 (マイクロ秒単位)
    int phase_us = 250;  // 初期値 (マイクロ秒単位)

    /*while (true) {
        std::cout << "\nCurrent Silencer Settings:\n";
        std::cout << "  Intensity: " << intensity << " us\n";
        std::cout << "  Phase: " << phase_us << " us\n";
        std::cout << "Options:\n";
        std::cout << "  1. Change intensity\n";
        std::cout << "  2. Change phase\n";
        std::cout << "  3. Apply settings\n";
        std::cout << "  4. Quit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 4) {
            std::cout << "Exiting silencer adjustment.\n";
            break;
        }

        switch (choice) {
        case 1: {
            std::cout << "Enter new intensity (multiple of 25): ";
            int new_intensity;
            std::cin >> new_intensity;

            if (new_intensity % 25 == 0 && new_intensity > 0) {
                intensity = new_intensity;
                std::cout << "Intensity updated to " << intensity << " us.\n";
            }
            else {
                std::cout << "Invalid intensity. Must be a positive multiple of 25.\n";
            }
            break;
        }
        case 2: {
            std::cout << "Enter new phase (multiple of 25): ";
            int new_phase;
            std::cin >> new_phase;

            if (new_phase % 25 == 0 && new_phase >= 0) {
                phase_us = new_phase;
                std::cout << "Phase updated to " << phase_us << " us.\n";
            }
            else {
                std::cout << "Invalid phase. Must be a non-negative multiple of 25.\n";
            }
            break;
        }
        case 3: {
            const auto config = autd3::Silencer{
                autd3::FixedCompletionTime{
                    .intensity = std::chrono::microseconds(intensity),
                    .phase = std::chrono::microseconds(phase_us)
                } };
            const auto is_valid = config.is_valid(m);
            if (is_valid) {
                autd3::Null nul;
                autd.send(nul);
                autd.send(config);
                autd.send((m, g));
                std::cout << "Silencer settings applied to AUTD.\n";
            }
            else {
                std::cout << "Invalid silencer settings. Please adjust intensity or phase.\n";
            }
            break;
        }
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }*/
}
