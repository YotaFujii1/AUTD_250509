#pragma once

#include <optional>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
class DatagramWithParallelThreshold final {
 public:
  DatagramWithParallelThreshold(D datagram, const std::optional<size_t> threshold) : _datagram(std::move(datagram)), _threshold(threshold) {}
  ~DatagramWithParallelThreshold() = default;                                                    // LCOV_EXCL_LINE
  DatagramWithParallelThreshold(const DatagramWithParallelThreshold& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramWithParallelThreshold& operator=(const DatagramWithParallelThreshold& obj) = default;  // LCOV_EXCL_LINE
  DatagramWithParallelThreshold(DatagramWithParallelThreshold&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramWithParallelThreshold& operator=(DatagramWithParallelThreshold&& obj) = default;       // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) const {
    const auto ptr = _datagram.ptr(g);
    return AUTDDatagramWithParallelThreshold(ptr, _threshold.has_value() ? static_cast<int32_t>(_threshold.value()) : -1);
  }

 private:
  D _datagram;
  std::optional<size_t> _threshold;
};

template <class D>
class IntoDatagramWithParallelThreshold {
 public:
  IntoDatagramWithParallelThreshold() = default;                                                         // LCOV_EXCL_LINE
  virtual ~IntoDatagramWithParallelThreshold() = default;                                                // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold(const IntoDatagramWithParallelThreshold& v) noexcept = default;      // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold& operator=(const IntoDatagramWithParallelThreshold& obj) = default;  // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold(IntoDatagramWithParallelThreshold&& obj) = default;                  // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold& operator=(IntoDatagramWithParallelThreshold&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] DatagramWithParallelThreshold<D> with_parallel_threshold(const std::optional<size_t> threshold) & {  // LCOV_EXCL_LINE
    return DatagramWithParallelThreshold<D>(*static_cast<D*>(this), threshold);                                                // LCOV_EXCL_LINE
  }  // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] DatagramWithParallelThreshold<D> with_parallel_threshold(const std::optional<size_t> threshold) && {
    return DatagramWithParallelThreshold<D>(std::move(*static_cast<D*>(this)), threshold);
  }
};

}  // namespace autd3::driver
