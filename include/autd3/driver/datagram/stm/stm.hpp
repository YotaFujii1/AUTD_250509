#pragma once

#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct ControlPoint {
  template <uint8_t N>
  friend struct ControlPoints;

  AUTD3_DEF_PROP(Vector3, point)
  AUTD3_DEF_PARAM_PHASE(ControlPoint, phase_offset)

  explicit ControlPoint(Vector3 point) : ControlPoint(std::move(point), Phase(0)) {}

 private:
  ControlPoint() : ControlPoint(Vector3::Zero()) {}
  explicit ControlPoint(Vector3 point, const Phase offset) : _point(std::move(point)), _phase_offset(offset) {}
};

template <uint8_t N>
struct ControlPoints {
 private:
  std::array<ControlPoint, N> _points = {};

 public:
  [[nodiscard]] const std::array<ControlPoint, N>& points() const { return _points; }

  AUTD3_DEF_PARAM_INTENSITY(ControlPoints<N>, intensity)

  explicit ControlPoints(std::array<ControlPoint, N> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(const std::array<Vector3, N>& points) : ControlPoints(points, std::numeric_limits<EmitIntensity>::max()) {}

 private:
  explicit ControlPoints(std::array<ControlPoint, N> points, const EmitIntensity intensity) : _points(std::move(points)), _intensity(intensity) {}
  explicit ControlPoints(const std::array<Vector3, N>& points, const EmitIntensity intensity) : _intensity(intensity) {
    for (size_t i = 0; i < N; i++) this->_points[i] = ControlPoint(points[i]);
  }
};

template <>
struct ControlPoints<1> {
 private:
  std::array<ControlPoint, 1> _points = {};

 public:
  [[nodiscard]] const std::array<ControlPoint, 1>& points() const { return _points; }

  AUTD3_DEF_PARAM_INTENSITY(ControlPoints<1>, intensity)

  explicit ControlPoints(std::array<ControlPoint, 1> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(const std::array<Vector3, 1>& points) : ControlPoints(points, std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(Vector3 point) : ControlPoints({std::move(point)}, std::numeric_limits<EmitIntensity>::max()) {}

 private:
  explicit ControlPoints(std::array<ControlPoint, 1> points, const EmitIntensity intensity) : _points(std::move(points)), _intensity(intensity) {}
  explicit ControlPoints(const std::array<Vector3, 1>& points, const EmitIntensity intensity) : _intensity(intensity) {
    for (size_t i = 0; i < 1; i++) this->_points[i] = ControlPoint(points[i]);
  }
};

}  // namespace autd3::driver