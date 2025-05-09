#pragma once

#include <concepts>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Static final : public driver::ModulationBase<Static>,
                     public driver::IntoModulationCache<Static>,
                     public driver::IntoRadiationPressure<Static>,
                     public driver::IntoFir<Static> {
 public:
  AUTD3_API Static() : _intensity(std::numeric_limits<uint8_t>::max()) {}

  template <std::integral I>
  AUTD3_API [[nodiscard]] static Static with_intensity(const I intensity) {
    return Static(static_cast<uint8_t>(intensity));
  }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override { return AUTDModulationStatic(_intensity, _loop_behavior); }

  AUTD3_DEF_PROP(uint8_t, intensity)

 private:
  AUTD3_API explicit Static(const uint8_t intensity) : _intensity(intensity) {}
};
}  // namespace autd3::modulation
