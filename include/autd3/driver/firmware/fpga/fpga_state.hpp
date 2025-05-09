#pragma once

#include <cstdint>
#include <optional>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class FPGAState {
 public:
  AUTD3_API explicit FPGAState(const uint8_t info) noexcept : _info(info) {}
  ~FPGAState() = default;                                // LCOV_EXCL_LINE
  FPGAState(const FPGAState& v) = default;               // LCOV_EXCL_LINE
  FPGAState& operator=(const FPGAState& obj) = default;  // LCOV_EXCL_LINE
  FPGAState(FPGAState&& obj) = default;                  // LCOV_EXCL_LINE
  FPGAState& operator=(FPGAState&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] bool is_thermal_assert() const noexcept { return (_info & (1 << 0)) != 0; }

  AUTD3_API [[nodiscard]] native_methods::Segment current_mod_segment() const noexcept {
    switch (_info & (1 << 1)) {
      case 0:
        return native_methods::Segment::S0;
      default:
        return native_methods::Segment::S1;
    }
  }

  AUTD3_API [[nodiscard]] std::optional<native_methods::Segment> current_stm_segment() const noexcept {
    if (!is_stm_mode()) {
      return std::nullopt;
    }
    switch (_info & (1 << 2)) {
      case 0:
        return native_methods::Segment::S0;
      default:
        return native_methods::Segment::S1;
    }
  }

  AUTD3_API [[nodiscard]] std::optional<native_methods::Segment> current_gain_segment() const noexcept {
    if (!is_gain_mode()) {
      return std::nullopt;
    }
    switch (_info & (1 << 2)) {
      case 0:
        return native_methods::Segment::S0;
      default:
        return native_methods::Segment::S1;
    }
  }

  AUTD3_API [[nodiscard]] bool is_gain_mode() const noexcept { return (_info & (1 << 3)) != 0; }

  AUTD3_API [[nodiscard]] bool is_stm_mode() const noexcept { return !is_gain_mode(); }

 private:
  uint8_t _info;
};

}  // namespace autd3::driver
