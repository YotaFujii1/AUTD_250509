#pragma once

#include "autd3/driver/datagram/with_segment.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class GainBase : public DatagramS<native_methods::GainPtr> {
 public:
  GainBase() = default;                                // LCOV_EXCL_LINE
  GainBase(const GainBase& obj) = default;             // LCOV_EXCL_LINE
  GainBase& operator=(const GainBase& obj) = default;  // LCOV_EXCL_LINE
  GainBase(GainBase&& obj) = default;                  // LCOV_EXCL_LINE
  GainBase& operator=(GainBase&& obj) = default;       // LCOV_EXCL_LINE
  ~GainBase() override = default;                      // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDGainIntoDatagram(gain_ptr(geometry));
  }

  AUTD3_API [[nodiscard]] native_methods::GainPtr raw_ptr(const geometry::Geometry& geometry) const override { return gain_ptr(geometry); }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(const native_methods::GainPtr p, const native_methods::Segment segment,
                                                                              const native_methods::TransitionModeWrap transition) const override {
    return AUTDGainIntoDatagramWithSegment(p, segment, transition);
  }

  AUTD3_API [[nodiscard]] virtual native_methods::GainPtr gain_ptr(const geometry::Geometry&) const = 0;
};

template <class G>
concept gain = std::derived_from<std::remove_reference_t<G>, GainBase>;

}  // namespace autd3::driver
