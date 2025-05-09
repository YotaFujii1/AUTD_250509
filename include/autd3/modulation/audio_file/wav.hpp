#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/resampler.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Wav final : public driver::ModulationBase<Wav>,
                  public driver::IntoModulationCache<Wav>,
                  public driver::IntoRadiationPressure<Wav>,
                  public driver::IntoFir<Wav> {
 public:
  AUTD3_API explicit Wav(std::filesystem::path path) : _path(std::move(path)) {}

  template <driver::sampling_config T>
  AUTD3_API explicit Wav(std::filesystem::path path, const T target, const SincInterpolation resampler)
      : _path(std::move(path)), _resample(std::make_tuple(target, resampler.dyn_resampler())) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(_resample.has_value() ? AUTDModulationAudioFileWavWithResample(_path.string().c_str(), _loop_behavior,
                                                                                   std::get<0>(_resample.value()), std::get<1>(_resample.value()))
                                          : AUTDModulationAudioFileWav(_path.string().c_str(), _loop_behavior));
  }

 private:
  std::filesystem::path _path;
  std::optional<std::tuple<driver::SamplingConfig, native_methods::DynSincInterpolator>> _resample;
};

}  // namespace autd3::modulation::audio_file
