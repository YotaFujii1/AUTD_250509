#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Bessel final : public driver::Gain<Bessel> {
 public:
  AUTD3_API explicit Bessel(driver::Vector3 p, driver::Vector3 d, const driver::Angle theta)
      : _pos(std::move(p)),
        _dir(std::move(d)),
        _theta(theta),
        _intensity(std::numeric_limits<driver::EmitIntensity>::max()),
        _phase_offset(driver::Phase(0)) {}
  Bessel() = delete;
  Bessel(const Bessel& obj) = default;             // LCOV_EXCL_LINE
  Bessel& operator=(const Bessel& obj) = default;  // LCOV_EXCL_LINE
  Bessel(Bessel&& obj) = default;                  // LCOV_EXCL_LINE
  Bessel& operator=(Bessel&& obj) = default;       // LCOV_EXCL_LINE
  ~Bessel() override = default;                    // LCOV_EXCL_LINE

  AUTD3_DEF_PROP(driver::Vector3, pos)
  AUTD3_DEF_PROP(driver::Vector3, dir)
  AUTD3_DEF_PROP(driver::Angle, theta)
  AUTD3_DEF_PARAM_INTENSITY(Bessel, intensity)
  AUTD3_DEF_PARAM_PHASE(Bessel, phase_offset)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainBessel(native_methods::Vector3{_pos.x(), _pos.y(), _pos.z()},
                                          native_methods::Vector3{_dir.x(), _dir.y(), _dir.z()}, _theta.radian(), _intensity.value(),
                                          _phase_offset.value());
  }
};

}  // namespace autd3::gain
