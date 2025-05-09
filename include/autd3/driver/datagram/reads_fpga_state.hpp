#pragma once

#include <concepts>

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class F>
concept _reads_fpga_info_f = requires(F f, const geometry::Device& d) {
  { f(d) } -> std::same_as<bool>;
};

template <_reads_fpga_info_f F>
class ReadsFPGAState final : public IntoDatagramTuple<ReadsFPGAState<F>>,
                             public IntoDatagramWithTimeout<ReadsFPGAState<F>>,
                             public IntoDatagramWithParallelThreshold<ReadsFPGAState<F>> {
  using native_f = bool (*)(const void*, native_methods::GeometryPtr, uint16_t);

 public:
  AUTD3_API explicit ReadsFPGAState(F f) : _f(std::move(f)) {
    _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> bool {
      const geometry::Device dev(dev_idx, geometry_ptr);
      return static_cast<const ReadsFPGAState*>(context)->_f(dev);
    };
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDDatagramReadsFPGAState(reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this), geometry.ptr());
  }

 private:
  F _f;
  native_f _f_native;
};

}  // namespace autd3::driver
