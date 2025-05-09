#pragma once

#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain {

class Null final : public driver::Gain<Null> {
 public:
  Null() = default;                            // LCOV_EXCL_LINE
  Null(const Null& obj) = default;             // LCOV_EXCL_LINE
  Null& operator=(const Null& obj) = default;  // LCOV_EXCL_LINE
  Null(Null&& obj) = default;                  // LCOV_EXCL_LINE
  Null& operator=(Null&& obj) = default;       // LCOV_EXCL_LINE
  ~Null() override = default;                  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainNull();
  }
};

}  // namespace autd3::gain
