#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/resampler.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Custom final : public driver::ModulationBase<Custom>,
                     public driver::IntoModulationCache<Custom>,
                     public driver::IntoRadiationPressure<Custom>,
                     public driver::IntoFir<Custom> {
 public:
  template <driver::sampling_config T>
  explicit Custom(std::vector<uint8_t> buf, const T sampling_config) : _sampling_config(sampling_config), _buf(std::move(buf)) {}

  template <driver::sampling_config T>
  explicit Custom(std::vector<uint8_t> buf, const driver::Freq<float> source, const T target, const SincInterpolation resampler)
      : _sampling_config(target), _buf(std::move(buf)), _resample(std::make_tuple(source.hz(), resampler.dyn_resampler())) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    const auto size = _buf.size();
    return _resample.has_value()
               ? AUTDModulationCustomWithResample(this->_loop_behavior, _buf.data(), static_cast<uint16_t>(size), std::get<0>(_resample.value()),
                                                  this->_sampling_config, std::get<1>(_resample.value()))
               : AUTDModulationCustom(this->_sampling_config, this->_loop_behavior, _buf.data(), static_cast<uint16_t>(size));
  }

 private:
  driver::SamplingConfig _sampling_config;
  std::vector<uint8_t> _buf;
  std::optional<std::tuple<float, native_methods::DynSincInterpolator>> _resample;
};

}  // namespace autd3::modulation
