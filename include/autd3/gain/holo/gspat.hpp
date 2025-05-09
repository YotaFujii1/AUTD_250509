#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <backend B>
class GSPAT final : public Holo<GSPAT<B>> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit GSPAT(std::shared_ptr<B> holo_backend, R&& iter)
      : Holo<GSPAT>(EmissionConstraint::Clamp(0x00, 0xFF), std::forward<R>(iter)), _repeat(100), _backend(std::move(holo_backend)) {}

  AUTD3_DEF_PARAM(GSPAT, uint32_t, repeat)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->gspat(reinterpret_cast<const float*>(this->_foci.data()), reinterpret_cast<const float*>(this->_amps.data()),
                                 static_cast<uint32_t>(this->_amps.size()), _repeat, this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
