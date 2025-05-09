#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct SwapSegment {
  class Gain final : public IntoDatagramTuple<Gain>, public IntoDatagramWithTimeout<Gain>, public IntoDatagramWithParallelThreshold<Gain> {
   public:
    AUTD3_API explicit Gain(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentGain(_segment, _transition_mode);
    }

    AUTD3_DEF_PROP(native_methods::Segment, segment)
    AUTD3_DEF_PROP(native_methods::TransitionModeWrap, transition_mode)
  };

  class GainSTM final : public IntoDatagramTuple<GainSTM>,
                        public IntoDatagramWithTimeout<GainSTM>,
                        public IntoDatagramWithParallelThreshold<GainSTM> {
   public:
    AUTD3_API explicit GainSTM(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentGainSTM(_segment, _transition_mode);
    }

    AUTD3_DEF_PROP(native_methods::Segment, segment)
    AUTD3_DEF_PROP(native_methods::TransitionModeWrap, transition_mode)
  };

  class FociSTM final : public IntoDatagramTuple<FociSTM>,
                        public IntoDatagramWithTimeout<FociSTM>,
                        public IntoDatagramWithParallelThreshold<FociSTM> {
   public:
    AUTD3_API explicit FociSTM(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentFociSTM(_segment, _transition_mode);
    }

    AUTD3_DEF_PROP(native_methods::Segment, segment)
    AUTD3_DEF_PROP(native_methods::TransitionModeWrap, transition_mode)
  };

  class Modulation final : public IntoDatagramTuple<Modulation>,
                           public IntoDatagramWithTimeout<Modulation>,
                           public IntoDatagramWithParallelThreshold<Modulation> {
   public:
    AUTD3_API explicit Modulation(const native_methods::Segment segment, const native_methods::TransitionModeWrap transition_mode)
        : _segment(segment), _transition_mode(transition_mode) {}

    AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const {
      return AUTDDatagramSwapSegmentModulation(_segment, _transition_mode);
    }

    AUTD3_DEF_PROP(native_methods::Segment, segment)
    AUTD3_DEF_PROP(native_methods::TransitionModeWrap, transition_mode)
  };
};

}  // namespace autd3::driver