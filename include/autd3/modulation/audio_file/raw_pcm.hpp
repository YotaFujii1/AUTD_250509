#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/resampler.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class RawPCM final : public driver::ModulationBase<RawPCM>,
                     public driver::IntoModulationCache<RawPCM>,
                     public driver::IntoRadiationPressure<RawPCM>,
                     public driver::IntoFir<RawPCM> {
 public:
  template <driver::sampling_config T>
  AUTD3_API explicit RawPCM(std::filesystem::path path, const T config) : _config(config), _path(std::move(path)) {}

  template <driver::sampling_config T>
  AUTD3_API explicit RawPCM(std::filesystem::path path, const driver::Freq<float> source, const T target, const SincInterpolation resampler)
      : _config(target), _path(std::move(path)), _resample(std::make_tuple(source.hz(), resampler.dyn_resampler())) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(_resample.has_value()
                        ? AUTDModulationAudioFileRawPCMWithResample(_path.string().c_str(), _loop_behavior, std::get<0>(_resample.value()), _config,
                                                                    std::get<1>(_resample.value()))
                        : AUTDModulationAudioFileRawPCM(_path.string().c_str(), _config, _loop_behavior));
  }

 private:
  driver::SamplingConfig _config;
  std::filesystem::path _path;
  std::optional<std::tuple<float, native_methods::DynSincInterpolator>> _resample;
};

}  // namespace autd3::modulation::audio_file
