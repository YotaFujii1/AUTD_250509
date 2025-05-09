#pragma once

#include "autd3/def.hpp"

namespace autd3::native_methods {

struct FfiFutureResultStatus {
  uint8_t _data[24];
};
struct LocalFfiFutureResultStatus {
  uint8_t _data[24];
};
struct FfiFutureResultFPGAStateList {
  uint8_t _data[24];
};
struct FfiFutureResultFirmwareVersionList {
  uint8_t _data[24];
};
struct FfiFutureResultController {
  uint8_t _data[24];
};

struct Vector3 {
  float x;
  float y;
  float z;

  operator driver::Vector3() const { return {x, y, z}; }
};

struct Quaternion {
  float x;
  float y;
  float z;
  float w;

  operator driver::Quaternion() const { return {w, x, y, z}; }
};

}  // namespace autd3::native_methods
