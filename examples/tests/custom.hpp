#pragma once

#include <autd3.hpp>
#include <autd3/gain/custom.hpp>

template <typename L>
inline void custom(autd3::Controller<L>& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  autd3::modulation::Sine m(150 * autd3::Hz);  // 150Hz AM

  const autd3::gain::Custom g([](const auto& dev) -> auto {
    return [&](const auto& tr) -> autd3::Drive {
      if (dev.idx() == 0 && tr.idx() == 0) return autd3::Drive(autd3::Phase(0), std::numeric_limits<autd3::EmitIntensity>::max());
      if (dev.idx() == 0 && tr.idx() == 248) return autd3::Drive(autd3::Phase(0), std::numeric_limits<autd3::EmitIntensity>::max());
      return autd3::Drive::null();
    };
  });
  autd.send((m, g));
}
