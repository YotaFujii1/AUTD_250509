#include "autd3/link/simulator.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

const float pi = 3.141592653589793238;
using autd3::rad;

// Run AUTD Simulator before running this example

int main() try {
    auto autd =
        autd3::ControllerBuilder({
        // 230917à»â∫12ë‰ç\ê¨
        autd3::AUTD3(autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH, autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, -pi / 4 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(0, autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-autd3::AUTD3::DEVICE_WIDTH, autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-(1 + std::sqrt(2) / 2) * autd3::AUTD3::DEVICE_WIDTH, autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, std::sqrt(2) / 2 * autd3::AUTD3::DEVICE_WIDTH))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, pi / 4 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-(1 + std::sqrt(2) / 2) * autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, std::sqrt(2) / 2 * autd3::AUTD3::DEVICE_WIDTH))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, pi / 4 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(0, -autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 1 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, -pi / 4 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 3 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, -pi / 4 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(0, -autd3::AUTD3::DEVICE_HEIGHT * 3 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 3 / 2, 0))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, 0 * rad, 0 * rad)),

        autd3::AUTD3(autd3::Vector3(-(1 + std::sqrt(2) / 2) * autd3::AUTD3::DEVICE_WIDTH, -autd3::AUTD3::DEVICE_HEIGHT * 3 / 2, std::sqrt(2) / 2 * autd3::AUTD3::DEVICE_WIDTH))
            .with_rotation(autd3::EulerAngles::ZYZ(0 * rad, pi / 4 * rad, 0 * rad)) })

        .open(autd3::link::Simulator::builder("127.0.0.1:8080"));
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
