#pragma once

#include <variant>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/modulation/sampling_mode.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::modulation {

class Fourier;

class Sine final : public driver::Modulation<Sine> {
  AUTD3_API explicit Sine(const std::variant<SamplingModeExact, SamplingModeExactFloat, SamplingModeNearest> freq)
      : Modulation(driver::SamplingConfig(10)),
        _intensity(std::numeric_limits<uint8_t>::max()),
        _offset(0x80),
        _phase(0.0 * driver::rad),
        _clamp(false),
        _freq(freq) {}

 public:
  friend Fourier;

  AUTD3_API explicit Sine(const driver::Freq<uint32_t> freq) : Sine(SamplingModeExact{freq}) {}
  AUTD3_API explicit Sine(const driver::Freq<float> freq) : Sine(SamplingModeExactFloat{freq}) {}
  AUTD3_API [[nodiscard]] static Sine nearest(const driver::Freq<float> freq) { return Sine(SamplingModeNearest{freq}); }

  AUTD3_DEF_PARAM_INT(Sine, uint8_t, intensity)
  AUTD3_DEF_PARAM_INT(Sine, uint8_t, offset)
  AUTD3_DEF_PARAM(Sine, driver::Angle, phase)
  AUTD3_DEF_PARAM(Sine, bool, clamp)
  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return std::visit([&](const auto& f) { return f.sine_ptr(_sampling_config, _intensity, _offset, _phase, _clamp, _loop_behavior); }, _freq);
  }

 private:
  std::variant<SamplingModeExact, SamplingModeExactFloat, SamplingModeNearest> _freq;
};

}  // namespace autd3::modulation
