#pragma once

#include <ranges>
#include <vector>

#include "autd3/driver/datagram/gain/gain.hpp"
#include "autd3/gain/holo/amplitude.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <class R>
concept holo_foci_range = std::ranges::viewable_range<R> && std::same_as<std::ranges::range_value_t<R>, std::pair<driver::Vector3, Amplitude>>;

template <class H>
class Holo : public driver::Gain<H> {
 public:
  template <holo_foci_range R>
  AUTD3_API explicit Holo(const native_methods::EmissionConstraintWrap value, const R& iter) : _constraint(value) {
    for (auto [focus, amp] : iter) {
      _foci.emplace_back(std::move(focus));
      _amps.emplace_back(amp);
    }
  }

  AUTD3_DEF_PARAM(H, native_methods::EmissionConstraintWrap, constraint)

 protected:
  std::vector<driver::Vector3> _foci;
  std::vector<Amplitude> _amps;
};

}  // namespace autd3::gain::holo