#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

template <backend B>
class Naive final : public Holo<Naive<B>> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit Naive(std::shared_ptr<B> holo_backend, R&& iter)
      : Holo<Naive>(EmissionConstraint::Clamp(0x00, 0xFF), std::forward<R>(iter)), _backend(std::move(holo_backend)) {}

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->naive(reinterpret_cast<const float*>(this->_foci.data()), reinterpret_cast<const float*>(this->_amps.data()),
                                 static_cast<uint32_t>(this->_amps.size()), this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
