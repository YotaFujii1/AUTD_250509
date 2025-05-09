#pragma once

#include <variant>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class R>
concept fourier_sine_range = std::ranges::viewable_range<R> && std::same_as<std::remove_reference_t<std::ranges::range_value_t<R>>, Sine>;

class Fourier final : public driver::ModulationBase<Fourier>,
                      public driver::IntoModulationCache<Fourier>,
                      public driver::IntoRadiationPressure<Fourier>,
                      public driver::IntoFir<Fourier> {
 public:
  template <fourier_sine_range R>
  AUTD3_API explicit Fourier(const R& iter) : _clamp(false), _scale_factor(std::nullopt), _offset(0) {
    for (Sine e : iter) _components.emplace_back(std::move(e));
  }

  AUTD3_API explicit Fourier(const std::initializer_list<Sine> components)
      : _clamp(false), _scale_factor(std::nullopt), _offset(0x00), _components(components) {}

  AUTD3_DEF_PARAM(Fourier, bool, clamp)
  AUTD3_DEF_PARAM(Fourier, std::optional<float>, scale_factor)
  AUTD3_DEF_PARAM(Fourier, uint8_t, offset)

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    const auto idx = _components[0]._freq.index();

    std::vector<native_methods::SamplingConfig> config;
    config.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(config), [&](const auto& m) { return m._sampling_config; });

    std::vector<uint8_t> intensity;
    intensity.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(intensity), [&](const auto& m) { return m._intensity; });

    std::vector<uint8_t> offset;
    offset.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(offset), [&](const auto& m) { return m._offset; });

    std::vector<float> phase;
    phase.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(phase), [&](const auto& m) { return m._phase.radian(); });

    const std::unique_ptr<bool[]> clamp(new bool[_components.size()]);
    std::ranges::transform(_components, clamp.get(), [&](const auto& m) { return m._clamp; });

    switch (idx) {
      case 0: {
        std::vector<SamplingModeExact> exact;
        exact.reserve(_components.size());
        std::ranges::transform(_components, std::back_inserter(exact), [&](const auto& m) { return std::get<0>(m._freq); });
        return SamplingModeExact::fourier_ptr(exact, config, intensity, offset, phase, clamp, this->_clamp, this->_scale_factor, this->_offset,
                                              this->_loop_behavior);
      }
      case 1: {
        std::vector<SamplingModeExactFloat> exact_float;
        exact_float.reserve(_components.size());
        std::ranges::transform(_components, std::back_inserter(exact_float), [&](const auto& m) { return std::get<1>(m._freq); });
        return SamplingModeExactFloat::fourier_ptr(exact_float, config, intensity, offset, phase, clamp, this->_clamp, this->_scale_factor,
                                                   this->_offset, this->_loop_behavior);
      }
      case 2: {
        std::vector<SamplingModeNearest> nearest;
        nearest.reserve(_components.size());
        std::ranges::transform(_components, std::back_inserter(nearest), [&](const auto& m) { return std::get<2>(m._freq); });
        return SamplingModeNearest::fourier_ptr(nearest, config, intensity, offset, phase, clamp, this->_clamp, this->_scale_factor, this->_offset,
                                                this->_loop_behavior);
      }
      default:                                    // LCOV_EXCL_LINE
        throw std::runtime_error("unreachable");  // LCOV_EXCL_LINE
    }
  }

 private:
  std::vector<Sine> _components;
};

}  // namespace autd3::modulation
