#pragma once

#include <limits>
#include <vector>

#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct SamplingModeExact {
  driver::Freq<uint32_t> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineExact(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<SamplingModeExact>& sine_freq,
                                                                 const std::vector<native_methods::SamplingConfig>& sine_config,
                                                                 const std::vector<uint8_t>& sine_intensity, const std::vector<uint8_t>& sine_offset,
                                                                 const std::vector<float>& sine_phase, const std::unique_ptr<bool[]>& sine_clamp,
                                                                 const bool clamp, const std::optional<float> scale_factor, const uint8_t offset,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    std::vector<uint32_t> freq;
    freq.reserve(sine_freq.size());
    std::ranges::transform(sine_freq, std::back_inserter(freq), [&](const auto& m) { return m.freq.hz(); });
    return validate(AUTDModulationFourierExact(freq.data(), sine_config.data(), sine_intensity.data(), sine_offset.data(), sine_phase.data(),
                                               sine_clamp.get(), static_cast<uint32_t>(sine_freq.size()), clamp,
                                               scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), offset, loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareExact(freq.hz(), config, low, high, duty, loop_behavior));
  }
};

struct SamplingModeExactFloat {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineExactFloat(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<SamplingModeExactFloat>& sine_freq,
                                                                 const std::vector<native_methods::SamplingConfig>& sine_config,
                                                                 const std::vector<uint8_t>& sine_intensity, const std::vector<uint8_t>& sine_offset,
                                                                 const std::vector<float>& sine_phase, const std::unique_ptr<bool[]>& sine_clamp,
                                                                 const bool clamp, const std::optional<float> scale_factor, const uint8_t offset,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    std::vector<float> freq;
    freq.reserve(sine_freq.size());
    std::ranges::transform(sine_freq, std::back_inserter(freq), [&](const auto& m) { return m.freq.hz(); });
    return validate(AUTDModulationFourierExactFloat(freq.data(), sine_config.data(), sine_intensity.data(), sine_offset.data(), sine_phase.data(),
                                                    sine_clamp.get(), static_cast<uint32_t>(sine_freq.size()), clamp,
                                                    scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), offset, loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareExactFloat(freq.hz(), config, low, high, duty, loop_behavior));
  }
};

struct SamplingModeNearest {
  driver::Freq<float> freq;

  [[nodiscard]] native_methods::ModulationPtr sine_ptr(const native_methods::SamplingConfig config, const uint8_t intensity, const uint8_t offset,
                                                       const driver::Angle phase, const bool clamp,
                                                       const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSineNearest(freq.hz(), config, intensity, offset, phase.radian(), clamp, loop_behavior));
  }

  [[nodiscard]] static native_methods::ModulationPtr fourier_ptr(const std::vector<SamplingModeNearest>& sine_freq,
                                                                 const std::vector<native_methods::SamplingConfig>& sine_config,
                                                                 const std::vector<uint8_t>& sine_intensity, const std::vector<uint8_t>& sine_offset,
                                                                 const std::vector<float>& sine_phase, const std::unique_ptr<bool[]>& sine_clamp,
                                                                 const bool clamp, const std::optional<float> scale_factor, const uint8_t offset,
                                                                 const native_methods::LoopBehavior loop_behavior) {
    std::vector<float> freq;
    freq.reserve(sine_freq.size());
    std::ranges::transform(sine_freq, std::back_inserter(freq), [&](const auto& m) { return m.freq.hz(); });
    return validate(AUTDModulationFourierNearest(freq.data(), sine_config.data(), sine_intensity.data(), sine_offset.data(), sine_phase.data(),
                                                 sine_clamp.get(), static_cast<uint32_t>(sine_freq.size()), clamp,
                                                 scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()), offset, loop_behavior));
  }

  [[nodiscard]] native_methods::ModulationPtr square_ptr(const native_methods::SamplingConfig config, const uint8_t low, const uint8_t high,
                                                         const float duty, const native_methods::LoopBehavior loop_behavior) const {
    return validate(AUTDModulationSquareNearest(freq.hz(), config, low, high, duty, loop_behavior));
  }
};

}  // namespace autd3::modulation
