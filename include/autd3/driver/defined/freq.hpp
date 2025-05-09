#pragma once

#include <cstdint>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class T>
class Freq {
 public:
  AUTD3_API [[nodiscard]] T hz() const { return _value; }

  AUTD3_API explicit Freq(const T value) : _value(value) {}

  auto operator<=>(const Freq&) const = default;  // LCOV_EXCL_LINE

 private:
  T _value;
};

template <>
class Freq<int> {
 public:
  AUTD3_API operator Freq<uint32_t>() const { return Freq<uint32_t>(static_cast<uint32_t>(_value)); }

  AUTD3_API explicit Freq(const int value) : _value(value) {}

  auto operator<=>(const Freq&) const = default;  // LCOV_EXCL_LINE

 private:
  int _value;
};

class UnitHz {
  template <class T>
  AUTD3_API friend Freq<T> operator*(T l, const UnitHz&) {
    return Freq<T>(l);
  }
};

class UnitkHz {
  template <class T>
  AUTD3_API Freq<T> friend operator*(T l, const UnitkHz&) {
    return Freq<T>(l * 1000);
  }
};

constexpr UnitHz Hz = UnitHz{};
constexpr UnitkHz kHz = UnitkHz{};

}  // namespace autd3::driver
