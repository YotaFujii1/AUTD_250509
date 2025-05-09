#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Synchronize final : public IntoDatagramTuple<Synchronize>,
                          public IntoDatagramWithTimeout<Synchronize>,
                          public IntoDatagramWithParallelThreshold<Synchronize> {
 public:
  AUTD3_API Synchronize() = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] static native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::AUTDDatagramSynchronize(); }
};

}  // namespace autd3::driver
