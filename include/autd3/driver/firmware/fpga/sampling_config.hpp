#pragma once

#include <chrono>

#include "autd3/driver/defined/freq.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct SamplingConfig final {
  static SamplingConfig nearest(const driver::Freq<float> f) noexcept {
    return SamplingConfig(native_methods::AUTDSamplingConfigFromFreqNearest(f.hz()));
  }
  template <typename Rep, typename P>
  static SamplingConfig nearest(const std::chrono::duration<Rep, P> period) noexcept {
    return SamplingConfig(native_methods::AUTDSamplingConfigFromPeriodNearest(native_methods::to_duration(period)));
  }

  operator native_methods::SamplingConfig() const { return _inner; }

  [[nodiscard]] uint32_t division() const { return AUTDSamplingConfigDivision(_inner); }
  [[nodiscard]] Freq<float> freq() const { return AUTDSamplingConfigFreq(_inner) * driver::Hz; }
  [[nodiscard]] std::chrono::nanoseconds period() const { return native_methods::from_duration(AUTDSamplingConfigPeriod(_inner)); }

  SamplingConfig(const Freq<uint32_t> f) : _inner(validate(native_methods::AUTDSamplingConfigFromFreq(f.hz()))) {}
  SamplingConfig(const Freq<float> f) : _inner(validate(native_methods::AUTDSamplingConfigFromFreqF(f.hz()))) {}
  template <typename Rep, typename P>
  SamplingConfig(const std::chrono::duration<Rep, P> period)
      : _inner(validate(native_methods::AUTDSamplingConfigFromPeriod(native_methods::to_duration(period)))) {}
  explicit SamplingConfig(const uint16_t div) : _inner(validate(native_methods::AUTDSamplingConfigFromDivision(div))) {}
  explicit SamplingConfig(const native_methods::SamplingConfig inner) : _inner(inner) {}

  bool operator==(const SamplingConfig& other) const { return _inner.division == other._inner.division; }

 private:
  native_methods::SamplingConfig _inner;
};

template <class C>
concept sampling_config = std::is_convertible_v<C, SamplingConfig>;

}  // namespace autd3::driver
