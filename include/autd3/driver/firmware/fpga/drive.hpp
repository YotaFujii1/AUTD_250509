#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"

namespace autd3::driver {
struct Drive final {
  Phase phase;
  EmitIntensity intensity;

  constexpr Drive() : phase(Phase(0x00)), intensity(EmitIntensity(0x00)) {}
  AUTD3_API explicit constexpr Drive(const Phase phase, const EmitIntensity intensity) : phase(phase), intensity(intensity) {}
  AUTD3_API explicit constexpr Drive(const EmitIntensity intensity, const Phase phase) : phase(phase), intensity(intensity) {}
  AUTD3_API explicit constexpr Drive(const Phase phase) : phase(phase), intensity(EmitIntensity(0xFF)) {}  // LCOV_EXCL_LINE
  AUTD3_API explicit constexpr Drive(const EmitIntensity intensity) : phase(Phase(0x00)), intensity(intensity) {}
  AUTD3_API [[nodiscard]] static Drive null() noexcept { return {}; }

  auto operator<=>(const Drive&) const = default;  // LCOV_EXCL_LINE
};

template <typename... Args>
concept drive = std::constructible_from<Drive, Args...>;

}  // namespace autd3::driver
