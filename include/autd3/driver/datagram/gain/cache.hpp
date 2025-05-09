#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

template <class G>
class Cache final : public driver::GainBase,
                    public driver::IntoDatagramTuple<Cache<G>>,
                    public driver::IntoDatagramWithSegment<native_methods::GainPtr, Cache<G>>,
                    public driver::IntoDatagramWithTimeout<Cache<G>>,
                    public driver::IntoDatagramWithParallelThreshold<Cache<G>> {
 public:
  AUTD3_API explicit Cache(G g) : _g(std::move(g)), _ptr(nullptr) {}

  Cache() = delete;
  Cache(const Cache& obj) = default;             // LCOV_EXCL_LINE
  Cache& operator=(const Cache& obj) = default;  // LCOV_EXCL_LINE
  Cache(Cache&& obj) = default;                  // LCOV_EXCL_LINE
  Cache& operator=(Cache&& obj) = default;       // LCOV_EXCL_LINE
  ~Cache() override = default;                   // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry& geometry) const override {
    if (!_ptr) {
      _ptr = std::shared_ptr<void>(const_cast<void*>(native_methods::AUTDGainCache(_g.gain_ptr(geometry))._0), [](const void* ptr) {
        if (!ptr) return;
        AUTDGainCacheFree(native_methods::GainCachePtr{ptr});
      });
    }
    return AUTDGainCacheClone(native_methods::GainCachePtr{_ptr.get()});
  }

 private:
  G _g;
  mutable std::shared_ptr<void> _ptr;
};
}  // namespace autd3::gain

namespace autd3::driver {
template <class G>
class IntoGainCache {
 public:
  IntoGainCache() = default;                                     // LCOV_EXCL_LINE
  IntoGainCache(const IntoGainCache& obj) = default;             // LCOV_EXCL_LINE
  IntoGainCache& operator=(const IntoGainCache& obj) = default;  // LCOV_EXCL_LINE
  IntoGainCache(IntoGainCache&& obj) = default;                  // LCOV_EXCL_LINE
  IntoGainCache& operator=(IntoGainCache&& obj) = default;       // LCOV_EXCL_LINE
  virtual ~IntoGainCache() = default;                            // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] gain::Cache<G> with_cache() & { return gain::Cache(*static_cast<G*>(this)); }  // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] gain::Cache<G> with_cache() && { return gain::Cache(std::move(*static_cast<G*>(this))); }
};

}  // namespace autd3::driver
