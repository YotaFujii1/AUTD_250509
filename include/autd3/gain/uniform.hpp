#pragma once

#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain {

class Uniform final : public driver::Gain<Uniform> {
 public:
  template <driver::drive... I>
  AUTD3_API explicit Uniform(const I... drive) : _drive(driver::Drive(drive...)) {}
  Uniform() = delete;
  Uniform(const Uniform& obj) = default;             // LCOV_EXCL_LINE
  Uniform& operator=(const Uniform& obj) = default;  // LCOV_EXCL_LINE
  Uniform(Uniform&& obj) = default;                  // LCOV_EXCL_LINE
  Uniform& operator=(Uniform&& obj) = default;       // LCOV_EXCL_LINE
  ~Uniform() override = default;                     // LCOV_EXCL_LINE

  AUTD3_DEF_PROP(driver::Drive, drive)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainUniform(_drive.intensity.value(), _drive.phase.value());
  }
};
}  // namespace autd3::gain
