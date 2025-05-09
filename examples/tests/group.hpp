#pragma once

#include "autd3.hpp"

template <typename T>
inline void group_by_device_test(autd3::Controller<T>& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  const autd3::Vector3 center = autd.center() + autd3::Vector3(0.0, 0.0, 150.0);

  autd.group([](const autd3::Device& dev) -> std::optional<const char*> {
        if (dev.idx() == 0) {
          return "null";
        } else if (dev.idx() == 1) {
          return "focus";
        } else {
          return std::nullopt;
        }
      })
      .set("null", (autd3::modulation::Static(), autd3::gain::Null()))
      .set("focus", (autd3::modulation::Sine(150 * autd3::Hz), autd3::gain::Focus(center)))
      .send();
}

template <typename T>
inline void group_by_transducer_test(autd3::Controller<T>& autd) {
  auto silencer = autd3::Silencer();
  autd.send(silencer);

  const auto cx = autd.center().x();
  autd3::gain::Focus g1(autd.center() + autd3::Vector3(0, 0, 150));
  autd3::gain::Null g2;

  const auto g = autd3::gain::Group([&cx](const autd3::Device&) {
                   return [&cx](const autd3::Transducer& tr) -> std::optional<const char*> {
                     if (tr.position().x() < cx) return "focus";
                     return "null";
                   };
                 })
                     .set("focus", g1)
                     .set("null", g2);

  autd3::modulation::Sine m(150 * autd3::Hz);  // 150Hz AM

  autd.send((m, g));
}
