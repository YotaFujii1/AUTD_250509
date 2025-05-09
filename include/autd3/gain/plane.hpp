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

class Plane final : public driver::Gain<Plane> {
 public:
  AUTD3_API explicit Plane(driver::Vector3 d)
      : _dir(std::move(d)), _phase_offset(driver::Phase(0)), _intensity(std::numeric_limits<driver::EmitIntensity>::max()) {}
  Plane() = delete;
  Plane(const Plane& obj) = default;             // LCOV_EXCL_LINE
  Plane& operator=(const Plane& obj) = default;  // LCOV_EXCL_LINE
  Plane(Plane&& obj) = default;                  // LCOV_EXCL_LINE
  Plane& operator=(Plane&& obj) = default;       // LCOV_EXCL_LINE
  ~Plane() override = default;                   // LCOV_EXCL_LINE

  AUTD3_DEF_PROP(driver::Vector3, dir)
  AUTD3_DEF_PARAM_PHASE(Plane, phase_offset)
  AUTD3_DEF_PARAM_INTENSITY(Plane, intensity)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainPlane(native_methods::Vector3{_dir.x(), _dir.y(), _dir.z()}, _intensity.value(), _phase_offset.value());
  }
};

}  // namespace autd3::gain
