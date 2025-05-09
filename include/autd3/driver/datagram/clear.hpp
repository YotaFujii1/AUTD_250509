#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {
class Clear final : public IntoDatagramTuple<Clear>, public IntoDatagramWithTimeout<Clear>, public IntoDatagramWithParallelThreshold<Clear> {
 public:
  AUTD3_API Clear() = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] static native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramClear(); }
};
}  // namespace autd3::driver
