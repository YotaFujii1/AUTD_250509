#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Focus final : public driver::Gain<Focus> {
 public:
  AUTD3_API explicit Focus(driver::Vector3 p)
      : _pos(std::move(p)), _intensity(std::numeric_limits<driver::EmitIntensity>::max()), _phase_offset(driver::Phase(0)) {}
  Focus() = delete;
  Focus(const Focus& obj) = default;             // LCOV_EXCL_LINE
  Focus& operator=(const Focus& obj) = default;  // LCOV_EXCL_LINE
  Focus(Focus&& obj) = default;                  // LCOV_EXCL_LINE
  Focus& operator=(Focus&& obj) = default;       // LCOV_EXCL_LINE
  ~Focus() override = default;                   // LCOV_EXCL_LINE

  AUTD3_DEF_PROP(driver::Vector3, pos)
  AUTD3_DEF_PARAM_INTENSITY(Focus, intensity)
  AUTD3_DEF_PARAM_PHASE(Focus, phase_offset)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainFocus(native_methods::Vector3{_pos.x(), _pos.y(), _pos.z()}, _intensity.value(), _phase_offset.value());
  }
};

}  // namespace autd3::gain
