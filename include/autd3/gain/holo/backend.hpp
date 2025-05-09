#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

class Backend {
 public:
  Backend() noexcept : _ptr(native_methods::BackendPtr{nullptr}) {}
  explicit Backend(const native_methods::BackendPtr ptr) noexcept : _ptr(ptr) {}
  Backend(const Backend& obj) = default;             // LCOV_EXCL_LINE
  Backend& operator=(const Backend& obj) = default;  // LCOV_EXCL_LINE
  Backend(Backend&& obj) = default;                  // LCOV_EXCL_LINE
  Backend& operator=(Backend&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~Backend() = default;                      // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::BackendPtr ptr() const { return _ptr; }

  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gs(const float* foci, const float* amps, uint32_t size, uint32_t repeat,
                                                             native_methods::EmissionConstraintWrap constraint) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gspat(const float* foci, const float* amps, uint32_t size, uint32_t repeat,
                                                                native_methods::EmissionConstraintWrap constraint) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr naive(const float* foci, const float* amps, uint32_t size,
                                                                native_methods::EmissionConstraintWrap constraint) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr lm(const float* foci, const float* amps, uint32_t size, float eps1, float eps2, float tau,
                                                             uint32_t k_max, const float* initial, uint32_t initial_size,
                                                             native_methods::EmissionConstraintWrap constraint) const = 0;

 protected:
  native_methods::BackendPtr _ptr;
};

template <class B>
concept backend = requires(B b) {
  { b.ptr() } -> std::same_as<native_methods::BackendPtr>;
};

}  // namespace autd3::gain::holo
