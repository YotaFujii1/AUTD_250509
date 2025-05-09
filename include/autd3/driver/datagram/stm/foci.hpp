#pragma once

#include <optional>
#include <ranges>

#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R, uint8_t N>
concept foci_range_c = std::ranges::viewable_range<R> && std::is_constructible_v<ControlPoints<N>, std::ranges::range_value_t<R>>;

class FociSTM final : public IntoDatagramTuple<FociSTM>,
                      public DatagramS<native_methods::FociSTMPtr>,
                      public IntoDatagramWithTimeout<FociSTM>,
                      public IntoDatagramWithParallelThreshold<FociSTM> {
  struct IControlPointsArray {
    virtual ~IControlPointsArray() = default;
    [[nodiscard]] virtual native_methods::FociSTMPtr ptr(native_methods::SamplingConfig config, native_methods::LoopBehavior loop_behavior) const = 0;
    [[nodiscard]] virtual uint8_t n() const = 0;
  };

  template <uint8_t N>
  struct ControlPointsArray final : IControlPointsArray {
   private:
    std::vector<ControlPoints<N>> _points;

   public:
    ~ControlPointsArray() override = default;
    explicit ControlPointsArray(std::vector<ControlPoints<N>> points) : _points(std::move(points)) {}
    [[nodiscard]] native_methods::FociSTMPtr ptr(const native_methods::SamplingConfig config,
                                                 native_methods::LoopBehavior loop_behavior) const override {
      return validate(AUTDSTMFoci(config, reinterpret_cast<const void*>(_points.data()), static_cast<uint16_t>(_points.size()), N, loop_behavior));
    }
    [[nodiscard]] uint8_t n() const override { return N; }
  };

 public:
  FociSTM() = delete;
  FociSTM(const FociSTM& obj) = default;             // LCOV_EXCL_LINE
  FociSTM& operator=(const FociSTM& obj) = default;  // LCOV_EXCL_LINE
  FociSTM(FociSTM&& obj) = default;                  // LCOV_EXCL_LINE
  FociSTM& operator=(FociSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~FociSTM() override = default;                     // LCOV_EXCL_LINE

#define AUTD3_FOCI_STM_CONSTRUCTOR(N)                                                                                  \
  template <stm_sampling_config T, foci_range_c<N> R>                                                                  \
  AUTD3_API explicit FociSTM(const T sampling, const R& iter) : _loop_behavior(LoopBehavior::Infinite) {               \
    std::vector<ControlPoints<N>> points;                                                                              \
    for (auto e : iter) points.emplace_back(ControlPoints<N>{std::move(e)});                                           \
    _sampling = STMSamplingConfig(sampling, static_cast<uint16_t>(points.size()));                                     \
    _points = std::make_shared<ControlPointsArray<N>>(std::move(points));                                              \
  }                                                                                                                    \
  template <foci_range_c<N> R>                                                                                         \
  AUTD3_API [[nodiscard]] static FociSTM nearest(const Freq<float> freq, const R& iter) {                              \
    std::vector<ControlPoints<N>> points;                                                                              \
    for (auto e : iter) points.emplace_back(ControlPoints<N>{std::move(e)});                                           \
    const auto n = static_cast<uint16_t>(points.size());                                                               \
    return FociSTM(STMSamplingConfig::nearest(freq, n), std::make_shared<ControlPointsArray<N>>(std::move(points)));   \
  }                                                                                                                    \
  template <typename Rep, typename P, foci_range_c<N> R>                                                               \
  AUTD3_API [[nodiscard]] static FociSTM nearest(const std::chrono::duration<Rep, P> period, const R& iter) {          \
    std::vector<ControlPoints<N>> points;                                                                              \
    for (auto e : iter) points.emplace_back(ControlPoints<N>{std::move(e)});                                           \
    const auto n = static_cast<uint16_t>(points.size());                                                               \
    return FociSTM(STMSamplingConfig::nearest(period, n), std::make_shared<ControlPointsArray<N>>(std::move(points))); \
  }
  AUTD3_FOCI_STM_CONSTRUCTOR(1)
  AUTD3_FOCI_STM_CONSTRUCTOR(2)
  AUTD3_FOCI_STM_CONSTRUCTOR(3)
  AUTD3_FOCI_STM_CONSTRUCTOR(4)
  AUTD3_FOCI_STM_CONSTRUCTOR(5)
  AUTD3_FOCI_STM_CONSTRUCTOR(6)
  AUTD3_FOCI_STM_CONSTRUCTOR(7)
  AUTD3_FOCI_STM_CONSTRUCTOR(8)
#undef AUTD3_FOCI_STM_CONSTRUCTOR

  AUTD3_API [[nodiscard]] native_methods::FociSTMPtr raw_ptr(const geometry::Geometry&) const override {
    return _points->ptr(_sampling, _loop_behavior);
  }
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::FociSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMFociIntoDatagramWithSegment(p, _points->n(), segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDSTMFociIntoDatagram(raw_ptr(geometry), _points->n());
  }

  AUTD3_API
  [[nodiscard]] DatagramWithSegment<native_methods::FociSTMPtr> with_segment(
      const native_methods::Segment segment, const std::optional<native_methods::TransitionModeWrap>& transition_mode) {
    return DatagramWithSegment<native_methods::FociSTMPtr>(std::make_unique<FociSTM>(std::move(*this)), segment, transition_mode);
  }

  AUTD3_DEF_PARAM(FociSTM, native_methods::LoopBehavior, loop_behavior)

  AUTD3_API [[nodiscard]] Freq<float> freq() const { return _sampling.freq(); }
  AUTD3_API [[nodiscard]] std::chrono::nanoseconds period() const { return _sampling.period(); }
  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const { return _sampling.sampling_config(); }

  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_intensity() const { return sampling_config(); }
  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_phase() const { return sampling_config(); }

 private:
  AUTD3_API explicit FociSTM(const STMSamplingConfig sampling, std::shared_ptr<IControlPointsArray> points)
      : _loop_behavior(LoopBehavior::Infinite), _points(std::move(points)), _sampling(sampling) {}

  std::shared_ptr<IControlPointsArray> _points;
  STMSamplingConfig _sampling;
};

}  // namespace autd3::driver
