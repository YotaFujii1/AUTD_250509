#pragma once

#include <memory>
#include <optional>

#include "autd3/driver/datagram/with_parallel_threshold.hpp"
#include "autd3/driver/datagram/with_timeout.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <typename P>
class DatagramS {
 public:
  DatagramS() = default;                                 // LCOV_EXCL_LINE
  AUTD3_API virtual ~DatagramS() = default;              // LCOV_EXCL_LINE
  DatagramS(const DatagramS& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramS& operator=(const DatagramS& obj) = default;  // LCOV_EXCL_LINE
  DatagramS(DatagramS&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramS& operator=(DatagramS&& obj) = default;       // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] virtual P raw_ptr(const geometry::Geometry&) const = 0;
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr into_segment_transition(P p, native_methods::Segment segment,
                                                                                      native_methods::TransitionModeWrap transition_mode) const = 0;
};

template <typename P>
class DatagramWithSegment final : public IntoDatagramWithTimeout<DatagramWithSegment<P>>,
                                  public IntoDatagramWithParallelThreshold<DatagramWithSegment<P>> {
 public:
  AUTD3_API explicit DatagramWithSegment(std::shared_ptr<DatagramS<P>> datagram, const native_methods::Segment segment,
                                         const std::optional<native_methods::TransitionModeWrap>& transition_mode)
      : _datagram(std::move(datagram)), _segment(segment), _transition_mode(transition_mode) {}
  ~DatagramWithSegment() override = default;                                 // LCOV_EXCL_LINE
  DatagramWithSegment(const DatagramWithSegment& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramWithSegment& operator=(const DatagramWithSegment& obj) = default;  // LCOV_EXCL_LINE
  DatagramWithSegment(DatagramWithSegment&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramWithSegment& operator=(DatagramWithSegment&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) const {
    const auto raw_ptr = _datagram->raw_ptr(g);
    const auto transition = _transition_mode.has_value() ? _transition_mode.value() : native_methods::AUTDTransitionModeNone();
    return _datagram->into_segment_transition(raw_ptr, _segment, transition);
  }

 private:
  std::shared_ptr<DatagramS<P>> _datagram;
  native_methods::Segment _segment;
  std::optional<native_methods::TransitionModeWrap> _transition_mode;
};

template <typename P, class D>
class IntoDatagramWithSegment {
 public:
  IntoDatagramWithSegment() = default;                                               // LCOV_EXCL_LINE
  virtual ~IntoDatagramWithSegment() = default;                                      // LCOV_EXCL_LINE
  IntoDatagramWithSegment(const IntoDatagramWithSegment& v) noexcept = default;      // LCOV_EXCL_LINE
  IntoDatagramWithSegment& operator=(const IntoDatagramWithSegment& obj) = default;  // LCOV_EXCL_LINE
  IntoDatagramWithSegment(IntoDatagramWithSegment&& obj) = default;                  // LCOV_EXCL_LINE
  IntoDatagramWithSegment& operator=(IntoDatagramWithSegment&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] DatagramWithSegment<P> with_segment(const native_methods::Segment segment,
                                                              std::optional<native_methods::TransitionModeWrap> transition_mode) & {
    return DatagramWithSegment<P>(std::make_shared<D>(*static_cast<D*>(this)), segment, std::move(transition_mode));
  }
  AUTD3_API [[nodiscard]] DatagramWithSegment<P> with_segment(const native_methods::Segment segment,
                                                              std::optional<native_methods::TransitionModeWrap> transition_mode) && {
    return DatagramWithSegment<P>(std::make_shared<D>(std::move(*static_cast<D*>(this))), segment, std::move(transition_mode));
  }
};

}  // namespace autd3::driver
