#pragma once

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <datagram D1, datagram D2>
class DatagramTuple final : public IntoDatagramWithTimeout<DatagramTuple<D1, D2>>, public IntoDatagramWithParallelThreshold<DatagramTuple<D1, D2>> {
 public:
  DatagramTuple(D1 d1, D2 d2) : _d1(std::move(d1)), _d2(std::move(d2)) {}
  ~DatagramTuple() override = default;                           // LCOV_EXCL_LINE
  DatagramTuple(const DatagramTuple& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramTuple& operator=(const DatagramTuple& obj) = default;  // LCOV_EXCL_LINE
  DatagramTuple(DatagramTuple&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramTuple& operator=(DatagramTuple&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return native_methods::AUTDDatagramTuple(_d1.ptr(geometry), _d2.ptr(geometry));
  }

 private:
  D1 _d1;
  D2 _d2;
};

template <class D>
class IntoDatagramTuple {
 public:
  IntoDatagramTuple() = default;
  virtual ~IntoDatagramTuple() = default;
  IntoDatagramTuple(const IntoDatagramTuple& v) noexcept = default;
  IntoDatagramTuple& operator=(const IntoDatagramTuple& obj) = default;
  IntoDatagramTuple(IntoDatagramTuple&& obj) = default;
  IntoDatagramTuple& operator=(IntoDatagramTuple&& obj) = default;

  template <datagram D2>
  AUTD3_API [[nodiscard]] DatagramTuple<D, D2> operator,(D2 d2) & {
    return DatagramTuple<D, D2>(*static_cast<D*>(this), d2);
  }

  template <datagram D2>
  AUTD3_API [[nodiscard]] DatagramTuple<D, D2> operator,(D2 d2) const& {
    return DatagramTuple<D, D2>(*static_cast<const D*>(this), d2);
  }

  template <datagram D2>
  AUTD3_API [[nodiscard]] DatagramTuple<D, D2> operator,(D2 d2) && {
    return DatagramTuple<D, D2>(std::move(*static_cast<D*>(this)), d2);
  }
};

}  // namespace autd3::driver
