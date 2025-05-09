#pragma once

#include <filesystem>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/resampler.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation::audio_file {

class Csv final : public driver::ModulationBase<Csv>,
                  public driver::IntoModulationCache<Csv>,
                  public driver::IntoRadiationPressure<Csv>,
                  public driver::IntoFir<Csv> {
 public:
  template <driver::sampling_config T>
  AUTD3_API explicit Csv(std::filesystem::path path, const T config) : _deliminator(','), _config(config), _path(std::move(path)) {}

  template <driver::sampling_config T>
  AUTD3_API explicit Csv(std::filesystem::path path, const driver::Freq<float> source, const T target, const SincInterpolation resampler)
      : _deliminator(','), _config(target), _path(std::move(path)), _resample(std::make_tuple(source.hz(), resampler.dyn_resampler())) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return validate(_resample.has_value()
                        ? AUTDModulationAudioFileCsvWithResample(_path.string().c_str(), _deliminator, _loop_behavior, std::get<0>(_resample.value()),
                                                                 _config, std::get<1>(_resample.value()))
                        : AUTDModulationAudioFileCsv(_path.string().c_str(), _config, _deliminator, _loop_behavior));
  }

  AUTD3_DEF_PARAM(Csv, uint8_t, deliminator)

 private:
  driver::SamplingConfig _config;
  std::filesystem::path _path;
  std::optional<std::tuple<float, native_methods::DynSincInterpolator>> _resample;
};

}  // namespace autd3::modulation::audio_file
