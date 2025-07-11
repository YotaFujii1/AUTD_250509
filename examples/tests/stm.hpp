#pragma once

#include <autd3.hpp>
#include <ranges>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <array>
#include <cmath>
#include <functional>
#include <complex>
#include <optional>
#include <memory> 
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"

const float PI = 3.141592653589793238;
using namespace std::ranges::views;
//using namespace autd3::driver;


template <typename L>
inline void focus_stm(autd3::Controller<L>& autd) {
    auto silencer = autd3::Silencer::disable();
    autd.send(silencer);


    autd3::modulation::Static m;

    const autd3::Vector3 center = autd3::Vector3(0, 0, 220);
    constexpr size_t points_num = 200;
    constexpr auto radius = 5.0f;
    autd3::FociSTM stm(1.0f * autd3::Hz, iota(0) | take(points_num) | transform([&](auto i) {
        const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
        autd3::Vector3 p = center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0);
        return p;
        }));

    autd.send((m, stm));
}

template <typename L>
inline void gain_stm(autd3::Controller<L>& autd) {
    const auto silencer = autd3::Silencer{
                            autd3::FixedCompletionTime{
                                .intensity = std::chrono::microseconds(250),
                                .phase = std::chrono::microseconds(250)} };
    autd.send(silencer);

    autd3::modulation::Static m;

    //const autd3::Vector3 center = autd.center() + autd3::Vector3(0, 0, 150);
    const autd3::Vector3 center = autd3::Vector3(0, 0, 210);
    float frequency = 1.0f;
    constexpr size_t points_num = 50;
    float radius = 5.0f; //[mm]
    size_t foci_num = 6;
    size_t division_num = 12;
    float speed; //[m/s]
    int ind;
    int amp_change;

    float ampList[12] = { 1,0.75,0.5,0.25,0.5,0.75,1,0.75,0.5,0.25,0.5,0.75 };
    std::cout << "Which Ver?(1 or 2)" << std::endl;
    std::cin >> ind;
    std::cout << "amp_change?" << std::endl;
    std::cin >> amp_change;
    std::cout << "Input speed,foci_num" << std::endl;
    std::cin >> speed >> foci_num;
    frequency = speed / (radius * 2 * autd3::pi * 1e-3);

    std::cout << frequency << std::endl;
    std::cin.ignore(100, '\n');

    const auto n = 40000;


    autd3::GainSTM stm(autd3::driver::STMSamplingConfig::nearest(frequency * autd3::Hz, points_num), iota(0) | take(points_num) | transform([&](auto i) {
        /*const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
        return autd3::gain::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0));*/
        auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
        std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;
        for (int j = 0; j < foci_num; j++) {
            float theta;
            if (ind == 1)theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(foci_num);
            else theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(division_num);
            auto amp = 5e6 * autd3::gain::holo::Pa;
            if (amp_change != 0) {
                amp = ampList[j] * 5e6 * autd3::gain::holo::Pa;
            }
            foci.push_back(std::make_pair(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0), amp));
        }
        return autd3::gain::holo::GSPAT(backend, foci);

        }));

    autd.send((m, stm));

    std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";

    while (true) {
        if (_kbhit()) {
            char response = _getch(); // Get input without requiring Enter
            std::cout << response << std::endl; // Echo the pressed key

            if (response == 'n') break;

            if (response == 'y') {
                int intensity = 250; // Default intensity in microseconds
                int phase_us = 250;  // Default phase in microseconds
                std::cout << "\nAdjusting Silencer Parameters:\n";
                std::cout << "Use arrow keys to adjust intensity and phase.\n";
                std::cout << "Intensity must be a multiple of 25.\n";
                std::cout << "Press 'n' to quit.\n";

                while (true) {
                    if (_kbhit()) {
                        char key = _getch();

                        // Backup current values
                        int prev_intensity = intensity;
                        int prev_phase_us = phase_us;

                        if (key == 72) { // Up arrow key
                            intensity += 25; // Increase intensity
                        }
                        else if (key == 80) { // Down arrow key
                            intensity = std::max(25, intensity - 25); // Decrease intensity
                        }
                        else if (key == 75) { // Left arrow key
                            phase_us = std::max(0, phase_us - 25); // Decrease phase by 25 us
                        }
                        else if (key == 77) { // Right arrow key
                            phase_us += 25; // Increase phase by 25 us
                        }
                        else if (key == 'n') { // Quit adjusting parameters
                            std::cout << "Returning to main menu.\n";
                            break;
                        }

                        // Prepare silencer configuration
                        const auto config = autd3::Silencer{
                            autd3::FixedCompletionTime{
                                .intensity = std::chrono::microseconds(intensity),
                                .phase = std::chrono::microseconds(phase_us)} };

                        // Check if the configuration is valid
                        const auto is_valid = config.is_valid(autd3::Sine(150 * autd3::Hz));
                        if (is_valid) {
                            autd.send(config);
                            std::cout << "Silencer parameters updated and sent to AUTD:\n";
                            std::cout << "  Intensity: " << intensity << " us\n";
                            std::cout << "  Phase: " << phase_us << " us\n";
                        }
                        else {
                            // Revert to previous values if invalid
                            intensity = prev_intensity;
                            phase_us = prev_phase_us;
                            std::cout << "Invalid configuration. Reverting to previous values:\n";
                            std::cout << "  Intensity: " << intensity << " us\n";
                            std::cout << "  Phase: " << phase_us << " us\n";
                        }
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Avoid busy-waiting
                }
            }
            else {
                std::cout << "Invalid input. Please press 'y' or 'n'.\n";
            }

            // 再び入力を促すメッセージを表示
            std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Avoid busy-waiting
    }
}






template <typename L>
inline void gain_stm_241227(autd3::Controller<L>& autd) {
    const auto silencer = autd3::Silencer{
                            autd3::FixedCompletionTime{
                                .intensity = std::chrono::microseconds(250),
                                .phase = std::chrono::microseconds(250)} };
    autd.send(silencer);

    autd3::modulation::Static m;

    const autd3::Vector3 center = autd3::Vector3(0, 0, 210);
    float frequency = 1.0f;
    constexpr size_t points_num = 50;
    float radius = 5.0f;  // [mm]
    size_t foci_num = 6;
    size_t division_num = 12;
    float speed;  // [m/s]
    int ind;

    std::cout << "Which Ver?(1 or 2)" << std::endl;
    std::cin >> ind;

    // ループで speed と foci_num を複数回変更可能にする
    bool continue_input = true;
    while (continue_input) {
        std::cout << "Input speed, foci_num (enter negative speed to exit): " << std::endl;
        std::cin >> speed >> foci_num;

        if (speed < 0) {
            continue_input = false;
            break;  // negative speed to exit loop
        }

        frequency = speed / (radius * 2 * autd3::pi * 1e-3);
        std::cout << "Calculated frequency: " << frequency << " Hz" << std::endl;

        const auto n = 40000;

        autd3::GainSTM stm(autd3::driver::STMSamplingConfig::nearest(frequency * autd3::Hz, points_num), iota(0) | take(points_num) | transform([&](auto i) {
            auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
            auto amp = 5e6 * autd3::gain::holo::Pa;
            std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;

            for (int j = 0; j < foci_num; j++) {
                float theta;
                if (ind == 1)
                    theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(foci_num);
                else
                    theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(division_num);

                foci.push_back(std::make_pair(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0), amp));
            }

            return autd3::gain::holo::GSPAT(backend, foci);
            }));

        autd.send((m, stm));
    }

    std::cout << "Exiting input loop." << std::endl;
}


template <typename L>
inline void gain_stm_250106(autd3::Controller<L>& autd) {
    const auto silencer = autd3::Silencer{
                            autd3::FixedUpdateRate{
                                .intensity = 1, // Intensityを1に設定
                                .phase = 1      // Phaseを1に設定
                            }
    };
    autd.send(silencer);

    autd3::modulation::Static m;
    const autd3::Vector3 center = autd3::Vector3(0, 0, 210);
    float frequency = 1.0f;
    constexpr size_t points_num = 50;
    float radius = 5.0f; //[mm]
    size_t foci_num = 6;
    size_t division_num = 12;
    float speed; //[m/s]
    int ind;
    int amp_change;
    float amp_base;

    float ampList[12] = { 1,0.9,0.8,0.7,0.8,0.9, 1,0.9,0.8,0.7,0.8,0.9 };
    std::cout << "Which Ver?(1 or 2)" << std::endl;
    std::cin >> ind;
    std::cout << "amp_change?" << std::endl;
    std::cin >> amp_change;
    std::cout << "Input speed,foci_num,amp_base" << std::endl;
    std::cin >> speed >> foci_num >> amp_base;
    frequency = speed / (radius * 2 * autd3::pi * 1e-3);

    std::cout << frequency << std::endl;
    std::cin.ignore(100, '\n');

    const auto n = 40000;

    autd3::GainSTM stm(autd3::driver::STMSamplingConfig::nearest(frequency * autd3::Hz, points_num), iota(0) | take(points_num) | transform([&](auto i) {
        /*const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
        return autd3::gain::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0));*/
        auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
        std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;
        for (int j = 0; j < foci_num; j++) {
            float theta;
            if (ind == 1)theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(foci_num);
            else theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(division_num);
            auto amp = amp_base * autd3::gain::holo::Pa;
            if (amp_change != 0) {
                amp = ampList[j] * amp_base * autd3::gain::holo::Pa;
            }
            foci.push_back(std::make_pair(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0), amp));
        }
        return autd3::gain::holo::Naive(backend, foci);

        }));

    autd.send((m, stm));

    std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";

    while (true) {
        if (_kbhit()) {
            char response = _getch(); // Get input without requiring Enter
            std::cout << response << std::endl; // Echo the pressed key

            if (response == 'n') break;

            if (response == 'y') {
                int intensity = 1; // Default intensity
                int phase = 1;     // Default phase
                std::cout << "\nAdjusting Silencer Parameters:\n";
                std::cout << "Use arrow keys to adjust intensity and phase.\n";
                std::cout << "Press 'n' to quit.\n";

                while (true) {
                    if (_kbhit()) {
                        char key = _getch();

                        // Backup current values
                        int prev_intensity = intensity;
                        int prev_phase = phase;

                        if (key == 72) { // Up arrow key
                            intensity += 1; // Increase intensity
                        }
                        else if (key == 80) { // Down arrow key
                            intensity = std::max(1, intensity - 1); // Decrease intensity
                        }
                        else if (key == 75) { // Left arrow key
                            phase = std::max(0, phase - 10); // Decrease phase
                        }
                        else if (key == 77) { // Right arrow key
                            phase += 10; // Increase phase
                        }
                        else if (key == 'n') { // Quit adjusting parameters
                            std::cout << "Returning to main menu.\n";
                            break;
                        }

                        // Prepare silencer configuration
                        const auto config = autd3::Silencer{
                            autd3::FixedUpdateRate{
                                .intensity = (uint16_t)intensity,
                                .phase = (uint16_t)phase
                            }
                        };

                        // Check if the configuration is valid
                        const auto is_valid = config.is_valid(autd3::Sine(150 * autd3::Hz));
                        if (is_valid) {
                            autd.send(config);
                            std::cout << "Silencer parameters updated and sent to AUTD:\n";
                            std::cout << "  Intensity: " << intensity << "\n";
                            std::cout << "  Phase: " << phase << "\n";
                        }
                        else {
                            // Revert to previous values if invalid
                            intensity = prev_intensity;
                            phase = prev_phase;
                            std::cout << "Invalid configuration. Reverting to previous values:\n";
                            std::cout << "  Intensity: " << intensity << "\n";
                            std::cout << "  Phase: " << phase << "\n";
                        }
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Avoid busy-waiting
                }
            }
            else {
                std::cout << "Invalid input. Please press 'y' or 'n'.\n";
            }

            // 再び入力を促すメッセージを表示
            std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Avoid busy-waiting
    }
}


std::vector<std::vector<uint8_t>> loadPhaseDataFromCSV(const std::string& filename) {
    std::vector<std::vector<uint8_t>> phaseData;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "ファイルを開けません: " << filename << std::endl;
        return phaseData;
    }

    while (std::getline(file, line)) {
        std::vector<uint8_t> frame;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            try {
                int value = std::stoi(cell);
                if (value < 0 || value > 255) {
                    throw std::out_of_range("uint8_t範囲外");
                }
                frame.push_back(static_cast<uint8_t>(value));
            }
            catch (const std::exception& e) {
                std::cerr << "変換エラー: " << cell << " (" << e.what() << ")" << std::endl;
                frame.push_back(0);  // エラー時は0にするなど適宜対処
            }
        }

        if (!frame.empty()) {
            phaseData.push_back(frame);
        }
    }

    return phaseData;
}
#include <conio.h> // getch を使うため
#include <iostream>
#include <limits>
// 他の必要なヘッダーを含めてください

template<typename L>
inline void stm_silencer_comparison(autd3::Controller<L>& autd) {
    int foci_num;
    std::cout << "focusNum?" << std::endl;
    std::cin >> foci_num;
    float frequency = 5;
    int points_num = 50;
    float radius = 10;
    float z = 200;
    uint8_t silencerStrength = 32;
    auto phaseData_post = loadPhaseDataFromCSV("C:/Users/shinolab/FujiiYota/autd3-cpp-main/examples/tests/phaseData_points/phaseData_phase20_delta32_points"+std::to_string(foci_num) + "_post.csv");
    auto phaseData = loadPhaseDataFromCSV("C:/Users/shinolab/FujiiYota/autd3-cpp-main/examples/tests/phaseData_points/phaseData_phase20_delta32_points"+std::to_string(foci_num)+".csv");
    auto phaseData_naive = loadPhaseDataFromCSV("C:/Users/shinolab/FujiiYota/autd3-cpp-main/examples/tests/phaseData_points/phaseData_phase20_delta32_points" + std::to_string(foci_num) + "_naive.csv");
    //phaseData = loadPhaseDataFromCSV("C:/Users/shinolab/FujiiYota/autd3-cpp-main/examples/tests/phaseData_points/phaseData_phase20_delta32_point4_filled.csv");

    while (true) {
        std::cout << "Press key (1:post_constrained(Naive), 2:post_constrained(GS), 3:pre_constrained(GS), 4:one-shot GS, q:quit): ";

        char ch = _getch();  // キー入力を即時取得
        std::cout << ch << std::endl; // 押されたキーの確認

        if (ch == 'q') break;

        //const auto silencer2 = autd3::Silencer::disable();
        //
        //autd.send(silencer2);
        /*const auto silencer = autd3::Silencer{
                autd3::FixedUpdateRate{.phase = silencerStrength }
        };*/
        const auto silencer = autd3::Silencer{
                autd3::FixedCompletionTime{
                .intensity = std::chrono::microseconds(1000),
                .phase = std::chrono::microseconds(4000)
        }
        };
        autd.send(silencer);

        

        


        if (ch == '1') {
            autd3::GainSTM stm(0.25f * autd3::Hz, iota(0) | take(phaseData_naive.size()) | transform([=](auto i) {
                const autd3::gain::Custom g([=](const auto& dev) -> auto {
                    return [&](const auto& tr) -> autd3::Drive {
                        size_t index = tr.idx() + tr.dev_idx() * 249;
                        uint8_t intensity = 255;
                        uint8_t phase = (256 - phaseData_naive[i][index]) % 256;
                        return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
                        };
                    });
                return g;
                }));

            autd.send(stm);
            autd.send(silencer);
            
        }
        else if (ch == '2') {
            autd3::GainSTM stm(0.25f * autd3::Hz, iota(0) | take(phaseData_post.size()) | transform([=](auto i) {
                const autd3::gain::Custom g([=](const auto& dev) -> auto {
                    return [&](const auto& tr) -> autd3::Drive {
                        size_t index = tr.idx() + tr.dev_idx() * 249;
                        uint8_t intensity = 255;
                        uint8_t phase = (256 - phaseData_post[i][index]) % 256;
                        return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
                        };
                    });
                return g;
                }));

            autd.send(stm);
            autd.send(silencer);
            
        }

        else if (ch == '3') {
            
            

            autd3::GainSTM stm(0.25f * autd3::Hz, iota(0) | take(phaseData.size()) | transform([=](auto i) {
                const autd3::gain::Custom g([=](const auto& dev) -> auto {
                    return [&](const auto& tr) -> autd3::Drive {
                        size_t index = tr.idx() + tr.dev_idx() * 249;
                        uint8_t intensity = 255;
                        uint8_t phase = (256 - phaseData[i][index]) % 256;
                        return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
                        };
                    });
                return g;
                }));

            autd.send(stm);
            autd.send(silencer);
            
        }

        else if (ch == '4') {
            auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
            auto amp = 1e7 * autd3::gain::holo::Pa;
            std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;
            for (int j = 0; j < foci_num; j++) {
                float theta = 2.0f * autd3::pi * j / foci_num;
                foci.emplace_back(autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), z), amp);
            }

            auto gain = autd3::gain::holo::GS(backend, foci);
            autd.send(gain);
            autd.send(silencer);
        }

        
    }
    
}

int ctoi(const char c) {
    if ('0' <= c && c <= '9') return (c - '0');
    return -1;
}

template<typename L>
inline void stm_silencer_comparison_points(autd3::Controller<L>& autd) {

    float frequency = 5;
    int points_num = 50;
    float radius = 10;
    float z = 200;
    uint8_t silencerStrength = 32;
  



    while (true) {
        std::cout << "Press key (1:post_constrained(Naive), 2:post_constrained(GS), 3:pre_constrained(GS), 4:one-shot GS, q:quit): ";

        char ch = _getch();  // キー入力を即時取得
        std::cout << ch << std::endl; // 押されたキーの確認

        if (ch == 'q') break;

        const auto silencer = autd3::Silencer{
                autd3::FixedUpdateRate{.phase = silencerStrength }
        };
        int foci_num = ctoi(ch);

        auto phaseData = loadPhaseDataFromCSV("C:/Users/shinolab/FujiiYota/autd3-cpp-main/examples/tests/phaseData_points/phaseData_phase20_delta32_points" + std::to_string(foci_num) + ".csv");
        
        autd3::GainSTM stm(1.0f * autd3::Hz, iota(0) | take(phaseData.size()) | transform([=](auto i) {
            const autd3::gain::Custom g([=](const auto& dev) -> auto {
                return [&](const auto& tr) -> autd3::Drive {
                    size_t index = tr.idx() + tr.dev_idx() * 249;
                    uint8_t intensity = 255;
                    uint8_t phase = (256 - phaseData[i][index]) % 256;
                    return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
                    };
                });
            return g;
            }));

        autd.send(stm);
        autd.send(silencer);



    }

}