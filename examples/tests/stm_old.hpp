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
#include<autd3/link/nop.hpp>
//using namespace std::ranges::views;

const float PI = 3.141592653589793238;
using namespace std::ranges::views;
using namespace autd3::driver;


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

template<typename L>
inline void stm_silencer_comparison(autd3::Controller<L>& autd) {
    std::cout << "Which? (1:post_constrained,2:pre_constrained)" << std::endl;

    int idx;
    std::cin >> idx;

    int foci_num = 4;
    float frequency = 5;
    int points_num = 50;
    float radius = 10;
    float z = 200;

    if (idx == 1) {
        const auto silencer = autd3::Silencer{
                                    autd3::FixedUpdateRate{
                                        .phase = 32
                                    }
        };
        autd3::GainSTM stm(frequency * autd3::Hz, iota(0) | take(points_num) | transform([&](auto i) {
            /*const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
            return autd3::gain::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0));*/
            auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
            auto amp = 5e6 * autd3::gain::holo::Pa;
            std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;
            for (int j = 0; j < foci_num; j++) {
                float theta;
                theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(foci_num);
                foci.push_back(std::make_pair(autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), z), amp));
            }
            return autd3::gain::holo::GSPAT(backend, foci);

            }));

        autd.send(silencer);
        autd.send(stm);
    }
    else if (idx == 2) {
        auto phaseData = loadPhaseDataFromCSV("C:/Users/MEIP-users/OneDrive - The University of Tokyo/デスクトップ/研究/sound-field-simulation-master/example/Fujii/phaseData.csv");

        //for (int i = 0; i < 10; i++) {
        //    for (int j = 0; j < 10; j++) {
        //        std::cout << (int)(phaseData[i][j]) << " ";
        //    }
        //    std::cout << std::endl;
        //}


        autd3::GainSTM stm(1.0f * autd3::Hz, iota(0) | take(points_num * frequency) | transform([=](auto i) {
 
            const autd3::gain::Custom g([=](const auto& dev) -> auto {
                return [&](const auto& tr) -> autd3::Drive {
                    size_t index = tr.idx() + tr.dev_idx() * 249;
                    //std::cout << (int)index << std::endl;
                    
                    uint8_t intensity = 255;
                  //uint8_t phase = phases[index];
                    uint8_t phase = (256 - phaseData[i][index]) % 256;
                    //if (i * 10 != index )intensity = 0;

                    return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
                    };
                });

            return g;
            }));

        autd.send(stm);

        //const autd3::gain::Custom g([=](const auto& dev) -> auto {
        //    return [&](const auto& tr) -> autd3::Drive {
        //        size_t index = tr.idx() + tr.dev_idx() * 249;
        //        //std::cout << (int)index << std::endl;

        //        uint8_t intensity = 255;
        //        //uint8_t phase = phases[index];
        //        uint8_t phase = (-phaseData[0][index] + 256) % 256;
        //        //if (i * 10 != index )intensity = 0;
        //        //std::cout << dev.wavenumber() << std::endl;

        //        return autd3::Drive(autd3::Phase(phase), autd3::EmitIntensity(intensity));
        //        };
        //    });

        //autd.send(g);




    }

    std::cin >> idx;
}
//
//template <typename L>
//inline void focus_stm(autd3::Controller<L>& autd) {
//    /*auto silencer = autd3::Silencer::disable();*/
//    std::vector<autd3::gain::Custom> custom_gains;
//    auto silencer = autd3::Silencer();
//    autd.send(silencer);
//
//    autd3::modulation::Static m;
//
//    const autd3::Vector3 center = autd.center() + autd3::Vector3(0, 0, 280);
//    constexpr size_t points_num = 200;
//    constexpr auto radius = 30.0f;
//    autd3::FociSTM stm(1.0f * autd3::Hz, iota(0) | take(points_num) | transform([&](auto i) {
//        const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
//        autd3::Vector3 p = center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0);
//        return p;
//        }));
//
//    autd.send((m, stm));
//}
//
////template <typename L>
////inline void gain_stm(autd3::Controller<L>& autd) {
////    const auto silencer = autd3::Silencer{
////                            autd3::FixedCompletionTime{
////                                .intensity = std::chrono::microseconds(250),
////                                .phase = std::chrono::microseconds(250)} };
////    autd.send(silencer);
////
////    autd3::modulation::Static m;
////
////    //const autd3::Vector3 center = autd.center() + autd3::Vector3(0, 0, 150);
////    const autd3::Vector3 center = autd3::Vector3(0, 0, 220);
////    float frequency = 1.0f;
////    constexpr size_t points_num = 50;
////    float radius = 5.0f; //[mm]
////    size_t foci_num = 6;
////    size_t division_num = 12;
////    float speed; //[m/s]
////    int ind;
////    std::cout << "Which Ver?(1 or 2)" << std::endl;
////    std::cin >> ind;
////    std::cout << "Input speed,foci_num" << std::endl;
////    std::cin >> speed >> foci_num;
////    frequency = speed / (radius * 2 * autd3::pi * 1e-3);
////
////    std::cout << frequency << std::endl;
////    std::cin.ignore(100, '\n');
////
////    const auto n = 40000;
////
////
////    autd3::GainSTM stm(autd3::driver::STMSamplingConfig::nearest(frequency * autd3::Hz, points_num), iota(0) | take(points_num) | transform([&](auto i) {
////        /*const auto theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num);
////        return autd3::gain::Focus(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0));*/
////        auto backend = std::make_shared<autd3::gain::holo::NalgebraBackend>();
////        auto amp = 5e6 * autd3::gain::holo::Pa;
////        std::vector<std::pair<autd3::Vector3, autd3::gain::holo::Amplitude>> foci;
////        for (int j = 0; j < foci_num; j++) {
////            float theta;
////            if (ind == 1)theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(foci_num);
////            else theta = 2.0f * autd3::pi * static_cast<float>(i) / static_cast<float>(points_num) + 2.0f * autd3::pi * static_cast<float>(j) / static_cast<float>(division_num);
////            foci.push_back(std::make_pair(center + autd3::Vector3(radius * std::cos(theta), radius * std::sin(theta), 0), amp));
////        }
////        return autd3::gain::holo::GSPAT(backend, foci);
////
////        }));
////
////    autd.send((m, stm));
////
////    std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";
////
////    while (true) {
////        if (_kbhit()) {
////            char response = _getch(); // Get input without requiring Enter
////            std::cout << response << std::endl; // Echo the pressed key
////
////            if (response == 'n') break;
////
////            if (response == 'y') {
////                int intensity = 250; // Default intensity in microseconds
////                int phase_us = 250;  // Default phase in microseconds
////                std::cout << "\nAdjusting Silencer Parameters:\n";
////                std::cout << "Use arrow keys to adjust intensity and phase.\n";
////                std::cout << "Intensity must be a multiple of 25.\n";
////                std::cout << "Press 'n' to quit.\n";
////
////                while (true) {
////                    if (_kbhit()) {
////                        char key = _getch();
////
////                        // Backup current values
////                        int prev_intensity = intensity;
////                        int prev_phase_us = phase_us;
////
////                        if (key == 72) { // Up arrow key
////                            intensity += 25; // Increase intensity
////                        }
////                        else if (key == 80) { // Down arrow key
////                            intensity = std::max(25, intensity - 25); // Decrease intensity
////                        }
////                        else if (key == 75) { // Left arrow key
////                            phase_us = std::max(0, phase_us - 25); // Decrease phase by 25 us
////                        }
////                        else if (key == 77) { // Right arrow key
////                            phase_us += 25; // Increase phase by 25 us
////                        }
////                        else if (key == 'n') { // Quit adjusting parameters
////                            std::cout << "Returning to main menu.\n";
////                            break;
////                        }
////
////                        // Prepare silencer configuration
////                        const auto config = autd3::Silencer{
////                            autd3::FixedCompletionTime{
////                                .intensity = std::chrono::microseconds(intensity),
////                                .phase = std::chrono::microseconds(phase_us)} };
////
////                        // Check if the configuration is valid
////                        const auto is_valid = config.is_valid(autd3::Sine(150 * autd3::Hz));
////                        if (is_valid) {
////                            autd.send(config);
////                            std::cout << "Silencer parameters updated and sent to AUTD:\n";
////                            std::cout << "  Intensity: " << intensity << " us\n";
////                            std::cout << "  Phase: " << phase_us << " us\n";
////                        }
////                        else {
////                            // Revert to previous values if invalid
////                            intensity = prev_intensity;
////                            phase_us = prev_phase_us;
////                            std::cout << "Invalid configuration. Reverting to previous values:\n";
////                            std::cout << "  Intensity: " << intensity << " us\n";
////                            std::cout << "  Phase: " << phase_us << " us\n";
////                        }
////                    }
////
////                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Avoid busy-waiting
////                }
////            }
////            else {
////                std::cout << "Invalid input. Please press 'y' or 'n'.\n";
////            }
////
////            // 再び入力を促すメッセージを表示
////            std::cout << "Press 'y' to adjust silencer, 'n' to quit: ";
////        }
////
////        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Avoid busy-waiting
////    }
////}
//
//// オーディオユニットの構造体
//struct Autd_data {
//    std::vector<std::array<float, 3>> pos;    // 各点の位置
//    std::vector<std::array<float, 3>> normal; // 各点の法線ベクトル
//};
//// グリーン関数
//std::complex<float> greenFunction(const std::array<float, 3>& dr, float k) {
//    float d = std::sqrt(dr[0] * dr[0] + dr[1] * dr[1] + dr[2] * dr[2]);
//    return std::exp(-std::complex<float>(0.0f, k * d)) / (4 * PI * d);
//}
//
//// 方向補完関数
//float directionalSpline(float theta) {
//    theta = std::abs(theta) / PI * 180;  // 度に変換
//    float dir_coef_a[] = { 1.0, 1.0, 1.0, 0.891250938, 0.707945784, 0.501187234, 0.354813389, 0.251188643, 0.199526231 };
//    float dir_coef_b[] = { 0, 0, -0.00459648054721, -0.0155520765675, -0.0208114779827, -0.0182211227016, -0.0122437497109, -0.00780345575475, -0.00312857467007 };
//    float dir_coef_c[] = { 0, 0, -0.000787968093807, -0.000307591508224, -0.000218348633296, 0.00047738416141, 0.000120353137658, 0.000323676257958, 0.000143811850511 };
//    float dir_coef_d[] = { 0, 0, 1.60125528528e-05, 2.9747624976e-06, 2.31910931569e-05, -1.1901034125e-05, 6.77743734332e-06, -5.99548024824e-06, -4.79372835035e-06 };
//
//    int i = std::floor(theta / 10) + 1;
//    if (i > 9) i = 9;
//
//    float a = dir_coef_a[i];
//    float b = dir_coef_b[i];
//    float c = dir_coef_c[i];
//    float d = dir_coef_d[i];
//    float x = theta - (i - 1) * 10;
//
//    float out = a + b * x + c * x * x + d * x * x * x;
//    if (out < 0) out = 0;
//    if (x > 10) out = 0;
//
//    return out;
//}
//
//// 伝達行列 G を作成する
//std::vector<std::vector<std::complex<float>>> makeMatrixG(const Autd_data& autd, const std::vector<std::array<float, 3>>& controlPos, float k, float alpha) {
//    //std::cout << "Creating Transfer Function Matrix G\n";
//
//    size_t N = autd.pos.size();
//    size_t M = controlPos.size();
//    std::vector<std::vector<std::complex<float>>> G(M, std::vector<std::complex<float>>(N, std::complex<float>(1.0f, 0.0f)));
//
//    for (size_t i = 0; i < M; ++i) {
//        for (size_t j = 0; j < N; ++j) {
//            // 各制御点とオーディオユニットの位置ベクトルとの差分を計算
//            std::array<float, 3> dr = {
//                controlPos[i][0] - autd.pos[j][0],
//                controlPos[i][1] - autd.pos[j][1],
//                controlPos[i][2] - autd.pos[j][2]
//            };
//
//            // 距離を計算
//            float d = std::sqrt(dr[0] * dr[0] + dr[1] * dr[1] + dr[2] * dr[2]);
//
//            // 内積を計算
//            float c = dr[0] * autd.normal[j][0] + dr[1] * autd.normal[j][1] + dr[2] * autd.normal[j][2];
//
//            // 角度を計算
//            float theta = std::acos(c / d);  // acos(c)は0 <= c <= 1 の範囲
//
//            // グリーン関数と方向補完を計算
//            G[i][j] = greenFunction(dr, k) * directionalSpline(theta) * std::exp(-alpha * d);
//        }
//    }
//
//    return G;
//}
//
//
//template <typename L>
//inline void silencer_greedy_STM(autd3::Controller<L>& autd) {
//    //variables
//    int focusNum = 10;
//    int divisionNum = 12;
//    int frameNum = 50;
//    float height = 200;
//    float rad = 5;
//    float phaseRange = 5;
//    //constants
//    const auto num_dev = autd.num_devices();
//    const auto num_tr = autd.num_transducers();
//    const auto num_tr_per_device = num_tr / num_dev;
//    const float waveNumber = 2 * PI / 8.5;
//    const float alpha = 0.0002;
//
//    const std::vector<uint8_t> ampList = {255};
//    //
//    Autd_data autd_info;
//    autd_info.pos.resize(num_tr);
//    autd_info.normal.resize(num_tr);
//
//    // 各デバイスおよびトランスデューサーの情報を設定
//    size_t tr_idx = 0; // トランスデューサーのインデックス
//
//    for (size_t dev_idx = 0; dev_idx < num_dev; ++dev_idx) {
//        auto dev = autd[dev_idx]; // デバイス取得
//        const auto normal = dev.rotation();
//        // 各トランスデューサーの設定
//        for (size_t tr_idx_in_dev = 0; tr_idx_in_dev < num_tr; ++tr_idx_in_dev) {
//            const auto tr = autd[dev_idx][tr_idx_in_dev]; // トランスデューサー取得
//
//            // トランスデューサーの位置を設定
//            const auto position = tr.position();
//            autd_info.pos[tr_idx] = { position.x(), position.y(), position.z() }; // 位置を設定
//
//            // 法線ベクトルを設定
//
//            autd_info.normal[tr_idx] = { normal.x(), normal.y(), normal.z() }; // 法線を設定
//
//            tr_idx++; // トランスデューサーインデックスの更新
//        }
//    }
//
//
//    //
//
//    auto silencer = autd3::Silencer::disable();
//    autd.send(silencer);
//
//    autd3::modulation::Static m;
//
//    std::vector<float> P_target(focusNum,5e6);
//
//    std::vector<autd3::gain::Custom> custom_gains;
//
//    bool firstTime = true;
//    std::vector<int> phase_before(num_tr);
//
//
//
//    // Customゲインを各時刻tで設定
//    for (size_t t = 0; t < frameNum; ++t) {
//        std::vector<std::vector<uint8_t>> phaseList(num_dev,std::vector<uint8_t>(num_tr_per_device));
//        std::vector<std::array<float, 3>> focusPos(focusNum);
//
//        for (int j = 0; j < focusNum; j++) {
//            float theta = 2 * PI * focusNum / divisionNum;
//            focusPos[j][0] = rad * std::cos(theta);
//            focusPos[j][1] = rad * std::sin(theta);
//            focusPos[j][2] = height;
//        }
//        auto G = makeMatrixG(autd_info, focusPos, waveNumber, alpha);
//
//        std::vector<std::complex<float>> Gq_Cache(focusNum, std::complex<float>(0.0f, 0.0f));
//        for (int n = 0; n < num_tr; n++) {
//            float E_min = 1e18;
//            int k_max, l_max;
//            for (int k = 0; k < ampList.size(); k++) {
//                if (firstTime) {
//                    for (int l = 0; l <= 255; l++) {
//                        l += 256;
//                        l %= 256;
//                        float E = 0;
//                        std::complex<float> q_i(ampList[k], (float)l / (2 * PI) * l);
//                        for (int m = 0; m < focusNum; m++) {
//                            E += std::pow(std::abs(std::abs(P_target[m]) - std::abs(Gq_Cache[l] + G[l][n] * q_i)), 2);
//                        }
//
//                        if (E_min > E) {
//                            k_max = k;
//                            l_max = l;
//                        }
//                    }
//                }
//                else {
//                    for (int l = phase_before[t] - rad; l <= phase_before[t] + rad; l++) {
//                        l += 256;
//                        l %= 256;
//                        float E = 0;
//                        std::complex<float> q_i(ampList[k], (float)l / (2 * PI) * l);
//                        for (int m = 0; m < focusNum; m++) {
//                            E += std::pow(std::abs(std::abs(P_target[m]) - std::abs(Gq_Cache[l] + G[l][n] * q_i)), 2);
//                        }
//
//                        if (E_min > E) {
//                            k_max = k;
//                            l_max = l;
//                        }
//                    }
//
//                }
//            }
//
//            phaseList[n / num_tr_per_device][n % num_tr_per_device] = l_max;
//            std::complex<float> q_i(ampList[k_max], (float)l_max / (2 * PI) * l_max);
//            for (int m = 0; m < focusNum; m++) {
//                Gq_Cache[m] += G[l_max][n] * q_i;
//            }
//
//            phase_before[n] = l_max;
//        }
//
//        auto res = autd3::gain::Custom(
//            [&phaseList,&num_tr_per_device](const auto& dev) {
//                return [&phaseList, &num_tr_per_device](const auto& tr) {
//                    return autd3::Drive(autd3::Phase(phaseList[tr.idx() / num_tr_per_device][tr.idx() % num_tr_per_device]), std::numeric_limits<autd3::EmitIntensity>::max());
//                    };
//            }
//        );
//
//        custom_gains.emplace_back(res);
//
//        firstTime = false;
//    }
//
//
//    // GainSTMのインスタンスを作成
//    autd3::driver::GainSTM stm(custom_gains, 1.0f * autd3::Hz);  // GainSTMMode は PhaseIntensityFull で初期化される
//
//    // stmMode の設定（もし必要ならば）
//    stm.mode = autd3::native_methods::GainSTMMode::PhaseIntensityFull;
//
//    // 送信
//    autd.send(m, stm);
//
//}