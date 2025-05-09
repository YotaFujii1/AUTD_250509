#pragma once

#include <memory>
#include <optional>
#include <ranges>

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R>
concept gain_range = std::ranges::viewable_range<R> && gain<std::ranges::range_value_t<R>>;

class GainSTM final : public IntoDatagramTuple<GainSTM>,
                      public DatagramS<native_methods::GainSTMPtr>,
                      public IntoDatagramWithTimeout<GainSTM>,
                      public IntoDatagramWithParallelThreshold<GainSTM> {
 public:
  GainSTM() = delete;
  GainSTM(const GainSTM& obj) = default;             // LCOV_EXCL_LINE
  GainSTM& operator=(const GainSTM& obj) = default;  // LCOV_EXCL_LINE
  GainSTM(GainSTM&& obj) = default;                  // LCOV_EXCL_LINE
  GainSTM& operator=(GainSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~GainSTM() override = default;                     // LCOV_EXCL_LINE

  template <stm_sampling_config T, gain_range R>
  AUTD3_API explicit GainSTM(const T sampling, const R& iter)
      : _loop_behavior(LoopBehavior::Infinite), _mode(native_methods::GainSTMMode::PhaseIntensityFull) {
    for (auto e : iter)
      _gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(std::forward<std::ranges::range_value_t<R>>(e)));
    _sampling = STMSamplingConfig(sampling, static_cast<uint16_t>(_gains.size()));
  }
  template <stm_sampling_config T, gain G>
  AUTD3_API explicit GainSTM(const T sampling, std::initializer_list<G> iter)
      : _loop_behavior(LoopBehavior::Infinite), _mode(native_methods::GainSTMMode::PhaseIntensityFull) {
    for (auto e : iter) _gains.emplace_back(std::make_shared<G>(e));
    _sampling = STMSamplingConfig(sampling, static_cast<uint16_t>(_gains.size()));
  }

  template <gain_range R>
  AUTD3_API [[nodiscard]] static GainSTM nearest(const Freq<float> freq, const R& iter) {
    std::vector<std::shared_ptr<GainBase>> gains;
    for (auto e : iter) gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(e));
    const auto n = static_cast<uint16_t>(gains.size());
    return GainSTM(STMSamplingConfig::nearest(freq, n), std::move(gains));
  }
  template <gain G>
  AUTD3_API [[nodiscard]] static GainSTM nearest(const Freq<float> freq, std::initializer_list<G> iter) {
    std::vector<std::shared_ptr<GainBase>> gains;
    for (auto e : iter) gains.emplace_back(std::make_shared<G>(e));
    const auto n = static_cast<uint16_t>(gains.size());
    return GainSTM(STMSamplingConfig::nearest(freq, n), std::move(gains));
  }
  template <typename Rep, typename P, gain_range R>
  AUTD3_API [[nodiscard]] static GainSTM nearest(const std::chrono::duration<Rep, P> period, const R& iter) {
    std::vector<std::shared_ptr<GainBase>> gains;
    for (auto e : iter) gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(e));
    const auto n = static_cast<uint16_t>(gains.size());
    return GainSTM(STMSamplingConfig::nearest(period, n), std::move(gains));
  }
  template <typename Rep, typename P, gain G>
  AUTD3_API [[nodiscard]] static GainSTM nearest(const std::chrono::duration<Rep, P> period, std::initializer_list<G> iter) {
    std::vector<std::shared_ptr<GainBase>> gains;
    for (auto e : iter) gains.emplace_back(std::make_shared<G>(e));
    const auto n = static_cast<uint16_t>(gains.size());
    return GainSTM(STMSamplingConfig::nearest(period, n), std::move(gains));
  }

  AUTD3_API [[nodiscard]] native_methods::GainSTMPtr raw_ptr(const geometry::Geometry& geometry) const override {
    std::vector<native_methods::GainPtr> gains;
    gains.reserve(_gains.size());
    std::ranges::transform(_gains, std::back_inserter(gains), [&](const auto& gain) { return gain->gain_ptr(geometry); });
    return validate(AUTDSTMGain(_sampling, gains.data(), static_cast<uint16_t>(gains.size()), _mode, _loop_behavior));
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::GainSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMGainIntoDatagramWithSegment(p, segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDSTMGainIntoDatagram(raw_ptr(geometry));
  }

  AUTD3_API
  [[nodiscard]] DatagramWithSegment<native_methods::GainSTMPtr> with_segment(
      const native_methods::Segment segment, const std::optional<native_methods::TransitionModeWrap>& transition_mode) {
    return DatagramWithSegment<native_methods::GainSTMPtr>(std::make_unique<GainSTM>(std::move(*this)), segment, transition_mode);
  }

  AUTD3_DEF_PARAM(GainSTM, native_methods::LoopBehavior, loop_behavior)
  AUTD3_DEF_PARAM(GainSTM, native_methods::GainSTMMode, mode)

  AUTD3_API [[nodiscard]] Freq<float> freq() const { return _sampling.freq(); }
  AUTD3_API [[nodiscard]] std::chrono::nanoseconds period() const { return _sampling.period(); }
  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const { return _sampling.sampling_config(); }

  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_intensity() const { return sampling_config(); }
  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_phase() const { return sampling_config(); }

 private:
  AUTD3_API explicit GainSTM(const STMSamplingConfig sampling, std::vector<std::shared_ptr<GainBase>> gains)
      : _loop_behavior(LoopBehavior::Infinite),
        _mode(native_methods::GainSTMMode::PhaseIntensityFull),
        _gains(std::move(gains)),
        _sampling(sampling) {}

  std::vector<std::shared_ptr<GainBase>> _gains;
  STMSamplingConfig _sampling;
};

}  // namespace autd3::driver
