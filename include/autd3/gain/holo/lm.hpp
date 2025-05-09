#pragma once

#include <memory>
#include <vector>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <backend B>
class LM final : public Holo<LM<B>> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit LM(std::shared_ptr<B> holo_backend, R&& iter)
      : Holo<LM>(EmissionConstraint::Clamp(0x00, 0xFF), std::forward<R>(iter)),
        _eps1(1e-8f),
        _eps2(1e-8f),
        _tau(1e-3f),
        _k_max(5),
        _backend(std::move(holo_backend)) {}

  AUTD3_DEF_PARAM(LM, float, eps1)
  AUTD3_DEF_PARAM(LM, float, eps2)
  AUTD3_DEF_PARAM(LM, float, tau)
  AUTD3_DEF_PARAM(LM, uint32_t, k_max)
  AUTD3_DEF_PARAM(LM, std::vector<float>, initial)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->lm(reinterpret_cast<const float*>(this->_foci.data()), reinterpret_cast<const float*>(this->_amps.data()),
                              static_cast<uint32_t>(this->_amps.size()), _eps1, _eps2, _tau, _k_max, _initial.data(),
                              static_cast<uint32_t>(_initial.size()), this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
