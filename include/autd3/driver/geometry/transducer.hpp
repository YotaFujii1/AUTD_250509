#pragma once

#include "autd3/def.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Transducer {
 public:
  AUTD3_API Transducer(const uint8_t idx, const uint16_t dev_idx, const native_methods::DevicePtr ptr)
      : _ptr(AUTDTransducer(ptr, idx)), _idx(idx), _dev_idx(dev_idx) {}

  AUTD3_API [[nodiscard]] Vector3 position() const noexcept { return AUTDTransducerPosition(_ptr); }

  AUTD3_API [[nodiscard]] size_t idx() const noexcept { return _idx; }
  AUTD3_API [[nodiscard]] size_t dev_idx() const noexcept { return _dev_idx; }

  AUTD3_API [[nodiscard]] native_methods::TransducerPtr ptr() const { return _ptr; }

 private:
  native_methods::TransducerPtr _ptr;
  uint8_t _idx;
  uint16_t _dev_idx;
};

}  // namespace autd3::driver::geometry
