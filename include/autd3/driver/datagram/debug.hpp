#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct DebugType {
  AUTD3_API static inline const native_methods::DebugTypeWrap None = native_methods::AUTDDebugTypeNone();
  AUTD3_API static inline const native_methods::DebugTypeWrap BaseSignal = native_methods::AUTDDebugTypeBaseSignal();
  AUTD3_API static inline const native_methods::DebugTypeWrap Thermo = native_methods::AUTDDebugTypeThermo();
  AUTD3_API static inline const native_methods::DebugTypeWrap ForceFan = native_methods::AUTDDebugTypeForceFan();
  AUTD3_API static inline const native_methods::DebugTypeWrap Sync = native_methods::AUTDDebugTypeSync();
  AUTD3_API static inline const native_methods::DebugTypeWrap ModSegment = native_methods::AUTDDebugTypeModSegment();
  AUTD3_API static native_methods::DebugTypeWrap ModIdx(const uint16_t idx) { return native_methods::AUTDDebugTypeModIdx(idx); }
  AUTD3_API static inline const native_methods::DebugTypeWrap StmSegment = native_methods::AUTDDebugTypeStmSegment();
  AUTD3_API static native_methods::DebugTypeWrap StmIdx(const uint16_t idx) { return native_methods::AUTDDebugTypeStmIdx(idx); }
  AUTD3_API static inline const native_methods::DebugTypeWrap IsStmMode = native_methods::AUTDDebugTypeIsStmMode();
  AUTD3_API static native_methods::DebugTypeWrap SysTimeEq(const DcSysTime sys_time) { return native_methods::AUTDDebugTypeSysTimeEq(sys_time); }
  AUTD3_API static native_methods::DebugTypeWrap PwmOut(const geometry::Transducer* tr) { return AUTDDebugTypePwmOut(tr->ptr()); }
  AUTD3_API static native_methods::DebugTypeWrap Direct(const bool v) { return native_methods::AUTDDebugTypeDirect(v); }
};

template <class F>
concept _debug_settings_f = requires(F f, const geometry::Device& d, const native_methods::GPIOOut gpio) {
  { f(d, gpio) } -> std::same_as<native_methods::DebugTypeWrap>;
};

template <_debug_settings_f F>
class DebugSettings final : public IntoDatagramTuple<DebugSettings<F>>,
                            public IntoDatagramWithTimeout<DebugSettings<F>>,
                            public IntoDatagramWithParallelThreshold<DebugSettings<F>> {
  using native_f = void (*)(const void*, native_methods::GeometryPtr, uint16_t, native_methods::GPIOOut, native_methods::DebugTypeWrap*);

 public:
  AUTD3_API explicit DebugSettings(F f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const native_methods::GPIOOut gpio,
                    native_methods::DebugTypeWrap* res) {
      const geometry::Device dev(dev_idx, geometry_ptr);
      *res = static_cast<const DebugSettings*>(context)->_f(dev, gpio);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDDatagramDebugSettings(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

 private:
  F _f;
  native_f _f_native;
};

}  // namespace autd3::driver
