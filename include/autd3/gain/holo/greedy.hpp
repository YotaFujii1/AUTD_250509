#pragma once

#include <vector>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/constraint.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

class Greedy final : public Holo<Greedy> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit Greedy(R&& iter)
      : Holo(EmissionConstraint::Uniform(std::numeric_limits<driver::EmitIntensity>::max()), std::forward<R>(iter)), _phase_div(16) {}

  AUTD3_DEF_PARAM(Greedy, uint8_t, phase_div)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return AUTDGainHoloGreedySphere(reinterpret_cast<const native_methods::Vector3*>(this->_foci.data()),
                                    reinterpret_cast<const float*>(this->_amps.data()), static_cast<uint32_t>(this->_amps.size()), _phase_div,
                                    _constraint);
  }
};

}  // namespace autd3::gain::holo
