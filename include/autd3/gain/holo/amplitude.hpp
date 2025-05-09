#pragma once

#include "autd3/native_methods.hpp"

#ifdef pascal
#undef pascal
#endif

namespace autd3::gain::holo {
class Amplitude;

class UnitPascal {
  friend Amplitude operator*(float l, const UnitPascal&);
};
class UnitSPL {
  friend Amplitude operator*(float l, const UnitSPL&);
};

constexpr UnitPascal Pa = UnitPascal{};
constexpr UnitSPL dB = UnitSPL{};

class Amplitude {
 public:
  friend class UnitPascal;
  friend class UnitSPL;

  AUTD3_API [[nodiscard]] float pascal() const { return _value; }
  AUTD3_API [[nodiscard]] float spl() const { return native_methods::AUTDGainHoloPascalToSPL(_value); }

  AUTD3_API friend Amplitude operator*(const float l, const UnitPascal&) { return Amplitude(l); }
  AUTD3_API friend Amplitude operator*(const float l, const UnitSPL&) { return Amplitude(native_methods::AUTDGainHoloSPLToPascal(l)); }

 private:
  AUTD3_API explicit Amplitude(const float value) : _value(value) {}

  float _value;
};

}  // namespace autd3::gain::holo