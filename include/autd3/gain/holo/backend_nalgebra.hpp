#pragma once

#include "autd3/gain/holo/backend.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class NalgebraBackend final : public Backend {
 public:
  NalgebraBackend() : Backend(native_methods::AUTDNalgebraBackendSphere()) {}
  AUTD3_API ~NalgebraBackend() override {
    if (_ptr._0 != nullptr) {
      AUTDDeleteNalgebraBackendSphere(_ptr);
      _ptr._0 = nullptr;
    }
  }
  NalgebraBackend(const NalgebraBackend& v) noexcept = delete;
  NalgebraBackend& operator=(const NalgebraBackend& obj) = delete;
  NalgebraBackend(NalgebraBackend&& obj) = default;             // LCOV_EXCL_LINE
  NalgebraBackend& operator=(NalgebraBackend&& obj) = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gs(const float* foci, const float* amps, const uint32_t size, const uint32_t repeat,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloGSSphere(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr gspat(const float* foci, const float* amps, const uint32_t size, const uint32_t repeat,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloGSPATSphere(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, repeat, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr naive(const float* foci, const float* amps, const uint32_t size,
                                                        const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloNaiveSphere(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, constraint);
  }
  AUTD3_API [[nodiscard]] native_methods::GainPtr lm(const float* foci, const float* amps, const uint32_t size, const float eps1, const float eps2,
                                                     const float tau, const uint32_t k_max, const float* initial, const uint32_t initial_size,
                                                     const native_methods::EmissionConstraintWrap constraint) const override {
    return AUTDGainHoloLMSphere(_ptr, reinterpret_cast<const native_methods::Vector3*>(foci), amps, size, eps1, eps2, tau, k_max, initial,
                                initial_size, constraint);
  }
};
}  // namespace autd3::gain::holo
