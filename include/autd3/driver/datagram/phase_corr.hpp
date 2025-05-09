#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class PhaseCorrection final : public IntoDatagramTuple<PhaseCorrection>,
                              public IntoDatagramWithTimeout<PhaseCorrection>,
                              public IntoDatagramWithParallelThreshold<PhaseCorrection> {
  using native_f = uint8_t (*)(const void*, native_methods::GeometryPtr, uint16_t, uint8_t);

 public:
  AUTD3_API explicit PhaseCorrection(std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx, const uint8_t tr_idx) -> uint8_t {
      auto* self = static_cast<PhaseCorrection*>(const_cast<void*>(context));
      bool contains;
      {
        std::shared_lock lock(self->_mtx);
        contains = self->_cache.contains(dev_idx);
      }
      const geometry::Transducer tr(tr_idx, dev_idx, AUTDDevice(geometry_ptr, dev_idx));
      const geometry::Device dev(dev_idx, geometry_ptr);
      if (contains) return self->_cache[dev_idx](tr).value();
      auto h = self->_f(dev);
      const auto res = h(tr);
      {
        std::lock_guard lock(self->_mtx);
        self->_cache[dev_idx] = std::move(h);
      }
      return res.value();
    };
  }

  PhaseCorrection(const PhaseCorrection& other) noexcept : _f(other._f), _f_native(other._f_native) {}        // LCOV_EXCL_LINE
  PhaseCorrection(PhaseCorrection&& other) noexcept : _f(std::move(other._f)), _f_native(other._f_native) {}  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDDatagramPhaseCorr(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

 private:
  std::function<std::function<Phase(const geometry::Transducer&)>(const geometry::Device&)> _f;
  native_f _f_native = nullptr;
  std::shared_mutex _mtx;
  std::unordered_map<uint16_t, std::function<Phase(const geometry::Transducer&)>> _cache;
};

}  // namespace autd3::driver
