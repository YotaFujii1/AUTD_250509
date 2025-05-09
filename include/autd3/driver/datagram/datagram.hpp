#pragma once

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
concept datagram = requires(D d, const geometry::Geometry& g) {
  { d.ptr(g) } -> std::same_as<native_methods::DatagramPtr>;
};

}  // namespace autd3::driver
