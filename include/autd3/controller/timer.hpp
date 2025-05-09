#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller::timer {

struct StdSleeper {
  std::optional<uint32_t> timer_resolution;
};

struct SpinSleeper {
  AUTD3_DEF_PROP(uint32_t, native_accuracy_ns)
  AUTD3_DEF_PARAM(SpinSleeper, native_methods::SpinStrategyTag, spin_strategy)
  explicit SpinSleeper(const uint32_t native_accuracy_ns = native_methods::AUTDTimerStrategySpinDefaultAccuracy())
      : _native_accuracy_ns(native_accuracy_ns),
        _spin_strategy(
#ifdef WIN32
            native_methods::SpinStrategyTag::SpinLoopHint
#else
            native_methods::SpinStrategyTag::YieldThread
#endif
        ) {
  }
};

struct AsyncSleeper {
  std::optional<uint32_t> timer_resolution;
};

struct WaitableSleeper {};

struct TimerStrategy {
  AUTD3_API static native_methods::TimerStrategyWrap Std(const StdSleeper sleeper) {
    return native_methods::AUTDTimerStrategyStd(sleeper.timer_resolution.value_or(0));
  }
  AUTD3_API static native_methods::TimerStrategyWrap Spin(const SpinSleeper sleeper) {
    return native_methods::AUTDTimerStrategySpin(sleeper.native_accuracy_ns(), sleeper.spin_strategy());
  }
  AUTD3_API static native_methods::TimerStrategyWrap Async(const AsyncSleeper sleeper) {
    return native_methods::AUTDTimerStrategyAsync(sleeper.timer_resolution.value_or(0));
  }
#ifdef WIN32
  AUTD3_API static native_methods::TimerStrategyWrap Waitable(WaitableSleeper) { return native_methods::AUTDTimerStrategyWaitable(); }
#endif
};

}  // namespace autd3::controller::timer
