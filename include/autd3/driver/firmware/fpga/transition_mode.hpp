#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class TransitionMode {
 public:
  AUTD3_API static inline const native_methods::TransitionModeWrap SyncIdx = native_methods::AUTDTransitionModeSyncIdx();
  AUTD3_API [[nodiscard]] static native_methods::TransitionModeWrap SysTime(const DcSysTime sys_time) {
    return native_methods::AUTDTransitionModeSysTime(sys_time);
  }
  AUTD3_API [[nodiscard]] static native_methods::TransitionModeWrap GPIO(const native_methods::GPIOIn gpio) { return AUTDTransitionModeGPIO(gpio); }
  AUTD3_API static inline const native_methods::TransitionModeWrap Ext = native_methods::AUTDTransitionModeExt();
  AUTD3_API static inline const native_methods::TransitionModeWrap Immediate = native_methods::AUTDTransitionModeImmediate();
};

}  // namespace autd3::driver
