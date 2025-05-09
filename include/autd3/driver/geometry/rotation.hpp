#pragma once

#include "autd3/driver/defined/angle.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class EulerAngles {
 public:
  AUTD3_API [[nodiscard]] static Quaternion XYZ(const Angle& x, const Angle& y, const Angle& z) {
    return native_methods::AUTDRotationFromEulerXYZ(x.radian(), y.radian(), z.radian());
  }
  AUTD3_API [[nodiscard]] static Quaternion ZYZ(const Angle& z1, const Angle& y, const Angle& z2) {
    return native_methods::AUTDRotationFromEulerZYZ(z1.radian(), y.radian(), z2.radian());
  }
};

}  // namespace autd3::driver::geometry
