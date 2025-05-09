#pragma once

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/gain/cache.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class G>
class Gain : public GainBase,
             public IntoDatagramTuple<G>,
             public IntoDatagramWithSegment<native_methods::GainPtr, G>,
             public IntoGainCache<G>,
             public IntoDatagramWithTimeout<G>,
             public IntoDatagramWithParallelThreshold<G> {
 public:
  Gain() = default;                            // LCOV_EXCL_LINE
  Gain(const Gain& obj) = default;             // LCOV_EXCL_LINE
  Gain& operator=(const Gain& obj) = default;  // LCOV_EXCL_LINE
  Gain(Gain&& obj) = default;                  // LCOV_EXCL_LINE
  Gain& operator=(Gain&& obj) = default;       // LCOV_EXCL_LINE
  ~Gain() override = default;                  // LCOV_EXCL_LINE
};

}  // namespace autd3::driver
