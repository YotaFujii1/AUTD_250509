#pragma once

#include <variant>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/modulation/sampling_mode.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Square final : public driver::Modulation<Square> {
  AUTD3_API explicit Square(const std::variant<SamplingModeExact, SamplingModeExactFloat, SamplingModeNearest> freq)
      : Modulation(driver::SamplingConfig(10)),
        _low(std::numeric_limits<uint8_t>::min()),
        _high(std::numeric_limits<uint8_t>::max()),
        _duty(0.5),
        _freq(freq) {}

 public:
  AUTD3_API explicit Square(const driver::Freq<uint32_t> freq) : Square(SamplingModeExact{freq}) {}
  AUTD3_API explicit Square(const driver::Freq<float> freq) : Square(SamplingModeExactFloat{freq}) {}
  AUTD3_API [[nodiscard]] static Square nearest(const driver::Freq<float> freq) { return Square(SamplingModeNearest{freq}); }

  AUTD3_DEF_PARAM_INT(Square, uint8_t, low)
  AUTD3_DEF_PARAM_INT(Square, uint8_t, high)
  AUTD3_DEF_PARAM(Square, float, duty)
  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    return std::visit([&](const auto& f) { return f.square_ptr(_sampling_config, _low, _high, _duty, _loop_behavior); }, _freq);
  }

 private:
  std::variant<SamplingModeExact, SamplingModeExactFloat, SamplingModeNearest> _freq;
};

}  // namespace autd3::modulation
