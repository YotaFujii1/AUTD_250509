#pragma once

#include <vector>

#include "autd3/def.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver::geometry {

class Device {
  class DeviceView : public std::ranges::view_interface<DeviceView> {
   public:
    DeviceView() = default;  // LCOV_EXCL_LINE
    AUTD3_API explicit DeviceView(const std::vector<Transducer>& vec) : _begin(vec.cbegin()), _end(vec.cend()) {}

    AUTD3_API [[nodiscard]] auto begin() const { return _begin; }
    AUTD3_API [[nodiscard]] auto end() const { return _end; }

   private:
    std::vector<Transducer>::const_iterator _begin{}, _end{};
  };

 public:
  AUTD3_API explicit Device(const uint16_t idx, const native_methods::GeometryPtr ptr) : _idx(static_cast<size_t>(idx)), _geo_ptr(ptr) {
    _ptr = AUTDDevice(ptr, idx);
    const auto size = static_cast<size_t>(AUTDDeviceNumTransducers(_ptr));
    _transducers.reserve(size);
    for (uint8_t i = 0; i < size; i++) _transducers.emplace_back(i, idx, _ptr);
  }

  ~Device() = default;                             // LCOV_EXCL_LINE
  Device(const Device& v) noexcept = default;      // LCOV_EXCL_LINE
  Device& operator=(const Device& obj) = default;  // LCOV_EXCL_LINE
  Device(Device&& obj) = default;                  // LCOV_EXCL_LINE
  Device& operator=(Device&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] size_t idx() const { return _idx; }

  AUTD3_API [[nodiscard]] size_t num_transducers() const { return _transducers.size(); }

  AUTD3_API [[nodiscard]] Vector3 center() const { return AUTDDeviceCenter(_ptr); }

  AUTD3_API [[nodiscard]] float sound_speed() const { return AUTDDeviceGetSoundSpeed(_ptr); }

  AUTD3_API void set_sound_speed(const float value) const { AUTDDeviceSetSoundSpeed(_geo_ptr, static_cast<uint16_t>(_idx), value); }

  AUTD3_API void set_sound_speed_from_temp(const float temp, const float k = 1.4f, const float r = 8.31446261815324f,
                                           const float m = 28.9647e-3f) const {
    AUTDDeviceSetSoundSpeedFromTemp(_geo_ptr, static_cast<uint16_t>(_idx), temp, k, r, m);
  }

  AUTD3_API [[nodiscard]] Quaternion rotation() const noexcept { return AUTDDeviceRotation(_ptr); }

  AUTD3_API [[nodiscard]] Vector3 x_direction() const { return AUTDDeviceDirectionX(_ptr); }

  AUTD3_API [[nodiscard]] Vector3 y_direction() const { return AUTDDeviceDirectionY(_ptr); }

  AUTD3_API [[nodiscard]] Vector3 axial_direction() const { return AUTDDeviceDirectionAxial(_ptr); }

  AUTD3_API [[nodiscard]] bool enable() const { return AUTDDeviceEnableGet(_ptr); }

  AUTD3_API void set_enable(const bool value) const { AUTDDeviceEnableSet(_geo_ptr, static_cast<uint16_t>(_idx), value); }

  AUTD3_API void translate(Vector3 t) const {
    AUTDDeviceTranslate(_geo_ptr, static_cast<uint16_t>(_idx), native_methods::Vector3{t.x(), t.y(), t.z()});
  }

  AUTD3_API void rotate(Quaternion r) const {
    AUTDDeviceRotate(_geo_ptr, static_cast<uint16_t>(_idx), native_methods::Quaternion{r.x(), r.y(), r.z(), r.w()});
  }

  AUTD3_API void affine(Vector3 t, Quaternion r) const {
    AUTDDeviceAffine(_geo_ptr, static_cast<uint16_t>(_idx), native_methods::Vector3{t.x(), t.y(), t.z()},
                     native_methods::Quaternion{r.x(), r.y(), r.z(), r.w()});
  }

  AUTD3_API [[nodiscard]] float wavelength() const { return AUTDDeviceWavelength(_ptr); }
  AUTD3_API [[nodiscard]] float wavenumber() const { return AUTDDeviceWavenumber(_ptr); }

  AUTD3_API [[nodiscard]] DeviceView transducers() const noexcept { return DeviceView(_transducers); }

  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator cbegin() const noexcept { return _transducers.cbegin(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator cend() const noexcept { return _transducers.cend(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator begin() const noexcept { return _transducers.begin(); }
  AUTD3_API [[nodiscard]] std::vector<Transducer>::const_iterator end() const noexcept { return _transducers.end(); }

  AUTD3_API [[nodiscard]] const Transducer& operator[](const size_t i) const { return _transducers[i]; }

  AUTD3_API [[nodiscard]] native_methods::DevicePtr ptr() const noexcept { return _ptr; }

 private:
  size_t _idx;
  native_methods::DevicePtr _ptr;
  native_methods::GeometryPtr _geo_ptr;
  std::vector<Transducer> _transducers{};
};

}  // namespace autd3::driver::geometry
