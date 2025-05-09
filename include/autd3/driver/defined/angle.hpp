#pragma once

#include <numbers>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class Angle;

class UnitRad {
  friend Angle operator*(float l, const UnitRad&);
};
class UnitDegree {
  friend Angle operator*(float l, const UnitDegree&);
};

constexpr UnitRad rad = UnitRad{};
constexpr UnitDegree deg = UnitDegree{};

class Angle {
 public:
  friend class UnitRad;
  friend class UnitDegree;

  AUTD3_API [[nodiscard]] float radian() const { return _value; }

  AUTD3_API friend Angle operator*(const float l, const UnitRad&) { return Angle(l); }
  AUTD3_API friend Angle operator*(const float l, const UnitDegree&) { return Angle(l / 180 * std::numbers::pi_v<float>); }

  auto operator<=>(const Angle&) const = default;  // LCOV_EXCL_LINE

 private:
  AUTD3_API explicit Angle(const float value) : _value(value) {}

  float _value;
};

}  // namespace autd3::driver
