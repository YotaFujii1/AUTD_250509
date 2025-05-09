#pragma once

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct LoopBehavior final {
  AUTD3_API static inline const native_methods::LoopBehavior Infinite = native_methods::AUTDLoopBehaviorInfinite();

  AUTD3_API [[nodiscard]] static native_methods::LoopBehavior Finite(const uint16_t v) {
    if (v == 0) throw AUTDException("Value must be greater than 0");
    return native_methods::AUTDLoopBehaviorFinite(v);
  }

  AUTD3_API static inline const native_methods::LoopBehavior Once = native_methods::AUTDLoopBehaviorOnce();
};

}  // namespace autd3::driver
