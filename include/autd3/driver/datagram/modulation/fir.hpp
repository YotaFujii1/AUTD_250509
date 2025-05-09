#pragma once

#include <ranges>

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/driver/datagram/modulation/cache.hpp"
#include "autd3/driver/datagram/modulation/radiation_pressure.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
class Fir final : public driver::ModulationBase<Fir<M>>, public driver::IntoModulationCache<Fir<M>>, public driver::IntoRadiationPressure<Fir<M>> {
 public:
  AUTD3_API explicit Fir(M m, std::vector<float> coef) : _m(std::move(m)), _coef(std::move(coef)) { this->_loop_behavior = _m.loop_behavior(); }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return native_methods::AUTDModulationWithFir(_m.modulation_ptr(), static_cast<native_methods::LoopBehavior>(this->_loop_behavior), _coef.data(),
                                                 static_cast<uint32_t>(_coef.size()));
  }

 private:
  M _m;
  std::vector<float> _coef;
};

}  // namespace autd3::modulation

namespace autd3::driver {

template <class R>
concept fir_coef_range = std::ranges::viewable_range<R> && std::same_as<std::remove_reference_t<std::ranges::range_value_t<R>>, float>;

template <class M>
class IntoFir {
 public:
  template <fir_coef_range R>
  AUTD3_API [[nodiscard]] modulation::Fir<M> with_fir(const R& coef) & {
    return modulation::Fir(*static_cast<M*>(this), std::vector(coef.begin(), coef.end()));
  }
  AUTD3_API [[nodiscard]] modulation::Fir<M> with_fir(const std::initializer_list<float> coef) & {
    return modulation::Fir(*static_cast<M*>(this), coef);
  }
  template <fir_coef_range R>
  AUTD3_API [[nodiscard]] modulation::Fir<M> with_fir(const R& coef) && {
    return modulation::Fir(std::move(*static_cast<M*>(this)), std::vector(coef.begin(), coef.end()));
  }
  AUTD3_API [[nodiscard]] modulation::Fir<M> with_fir(const std::initializer_list<float> coef) && {
    return modulation::Fir(std::move(*static_cast<M*>(this)), coef);
  }
};

}  // namespace autd3::driver
