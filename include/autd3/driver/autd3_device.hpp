#pragma once

#include <algorithm>

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

class AUTD3 {
 public:
  static constexpr size_t NUM_TRANS_IN_UNIT = native_methods::NUM_TRANS_IN_UNIT;
  static constexpr size_t NUM_TRANS_IN_X = native_methods::NUM_TRANS_IN_X;
  static constexpr size_t NUM_TRANS_IN_Y = native_methods::NUM_TRANS_IN_Y;
  static constexpr float TRANS_SPACING = native_methods::TRANS_SPACING_MM;
  static constexpr float DEVICE_WIDTH = native_methods::DEVICE_WIDTH_MM;
  static constexpr float DEVICE_HEIGHT = native_methods::DEVICE_HEIGHT_MM;

  AUTD3_API explicit AUTD3(Vector3 pos) : _position(std::move(pos)), _rotation(Quaternion::Identity()) {}

  AUTD3_DEF_PROP(Vector3, position)
  AUTD3_DEF_PARAM(AUTD3, Quaternion, rotation)
};
}  // namespace autd3::driver
