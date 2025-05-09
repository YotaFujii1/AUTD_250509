#pragma once

#include <chrono>
#include <future>
#include <optional>
#include <vector>

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/firmware/fpga/fpga_state.hpp"
#include "autd3/driver/firmware_version.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {

class ControllerBuilder;

template <class F>
concept group_f =
    requires(F f, const driver::geometry::Device& dev) { typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type; };

template <typename L>
class Controller final : public driver::geometry::Geometry {
  friend class ControllerBuilder;

 public:
  Controller() = delete;
  Controller(const Controller& v) = delete;
  Controller& operator=(const Controller& obj) = delete;
  Controller(Controller&& obj) noexcept
      : Geometry(obj._geometry_ptr, std::move(obj._devices)),
        _runtime(obj._runtime),
        _handle(obj._handle),
        _ptr(obj._ptr),
        _link(std::move(obj._link)) {
    obj._geometry_ptr._0 = nullptr;
    obj._runtime._0 = nullptr;
    obj._handle._0 = nullptr;
    obj._ptr._0 = nullptr;
  }
  Controller& operator=(Controller&& obj) noexcept {
    if (this != &obj) {
      this->_geometry_ptr = obj._geometry_ptr;
      this->_devices = std::move(obj._devices);
      _runtime = obj._runtime;
      _handle = obj._handle;
      _ptr = obj._ptr;
      _link = std::move(obj._link);
      obj._geometry_ptr._0 = nullptr;
      obj._runtime._0 = nullptr;
      obj._handle._0 = nullptr;
      obj._ptr._0 = nullptr;
    }
    return *this;
  }

  ~Controller() noexcept override {
    try {
      close();
    } catch (...) {  // LCOV_EXCL_LINE
    }  // LCOV_EXCL_LINE
  }

  AUTD3_API [[nodiscard]] const Geometry& geometry() const { return static_cast<const Geometry&>(*this); }  // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] Geometry& geometry() { return static_cast<Geometry&>(*this); }

  AUTD3_API [[nodiscard]] L& link() { return _link; }
  AUTD3_API [[nodiscard]] const L& link() const { return _link; }  // LCOV_EXCL_LINE

  AUTD3_API void close() {
    if (_ptr._0 == nullptr) return;
    const auto res = AUTDWaitResultStatus(_handle, AUTDControllerClose(_ptr));
    _ptr._0 = nullptr;
    AUTDDeleteRuntime(_runtime);
    _runtime._0 = nullptr;
    _handle._0 = nullptr;
    validate(res);
  }

  AUTD3_API [[nodiscard]] std::vector<std::optional<driver::FPGAState>> fpga_state() {
    const size_t num_devices = geometry().num_devices();
    const auto p = validate(AUTDWaitResultFPGAStateList(_handle, AUTDControllerFPGAState(_ptr)));
    std::vector<std::optional<driver::FPGAState>> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      const auto state = AUTDControllerFPGAStateGet(p, i);
      ret.emplace_back(state < 0 ? std::nullopt : std::optional(driver::FPGAState(static_cast<uint8_t>(state))));
    }
    AUTDControllerFPGAStateDelete(p);
    return ret;
  }  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] std::vector<driver::FirmwareVersion> firmware_version() {
    const size_t num_devices = geometry().num_devices();
    const auto handle = validate(AUTDWaitResultFirmwareVersionList(_handle, AUTDControllerFirmwareVersionListPointer(_ptr)));
    std::vector<driver::FirmwareVersion> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      char info[256]{};
      AUTDControllerFirmwareVersionGet(handle, i, info);
      ret.emplace_back(std::string(info));
    }
    AUTDControllerFirmwareVersionListPointerDelete(handle);
    return ret;
  }  // LCOV_EXCL_LINE

  template <driver::datagram D>
  AUTD3_API void send(const D& d) {
    validate(AUTDWaitResultStatus(_handle, AUTDControllerSend(_ptr, d.ptr(geometry()))));
  }

  template <group_f F>
  class GroupGuard {
   public:
    using key_type = typename std::invoke_result_t<F, const driver::geometry::Device&>::value_type;
    using native_f = int32_t (*)(const void*, native_methods::GeometryPtr, uint16_t);

    AUTD3_API explicit GroupGuard(F map, Controller& controller) : _controller(controller), _map(std::move(map)) {
      _f_native = +[](const void* context, const native_methods::GeometryPtr geometry_ptr, const uint16_t dev_idx) -> int32_t {
        const driver::geometry::Device dev(dev_idx, geometry_ptr);
        const auto* self = static_cast<const GroupGuard*>(context);
        const auto key = self->_map(dev);
        const auto& keymap = self->_keymap;
        return key.has_value() ? keymap.at(key.value()) : -1;
      };
    }

    template <driver::datagram D>
    AUTD3_API GroupGuard set(const key_type key, const D& data) {
      if (_keymap.contains(key)) throw AUTDException("Key already exists");
      const auto ptr = data.ptr(_controller.geometry());
      _datagrams.push_back(ptr);
      _keys.push_back(_k);
      _keymap[key] = _k++;
      return std::move(*this);
    }

    AUTD3_API void send() const {
      validate(AUTDWaitLocalResultStatus(
          _controller._handle, AUTDControllerGroup(_controller._ptr, reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this),
                                                   _controller._geometry_ptr, _keys.data(), _datagrams.data(), static_cast<uint16_t>(_keys.size()))));
    }

#ifdef AUTD3_ASYNC_API
    AUTD3_API [[nodiscard]] coro::task<void> send_async() {
      auto future = AUTDControllerGroup(_controller._ptr, reinterpret_cast<const void*>(_f_native), static_cast<const void*>(this),
                                        _controller._geometry_ptr, _keys.data(), _datagrams.data(), static_cast<uint16_t>(_keys.size()));
      auto ptr = co_await wait_future(_controller._handle, std::move(future));
      validate(ptr);
    }
#endif

   private:
    Controller& _controller;
    F _map;
    native_f _f_native;
    std::vector<int32_t> _keys;
    std::vector<native_methods::DatagramPtr> _datagrams;
    std::unordered_map<key_type, int32_t> _keymap;
    int32_t _k{0};
  };

  template <group_f F>
  AUTD3_API GroupGuard<F> group(const F& map) {
    return GroupGuard<F>(map, *this);
  }

#ifdef AUTD3_ASYNC_API
  AUTD3_API [[nodiscard]] coro::task<void> close_async() {
    if (_ptr._0 == nullptr) co_return;
    auto future = AUTDControllerClose(_ptr);
    auto res = co_await wait_future(_handle, std::move(future));
    _ptr._0 = nullptr;
    AUTDDeleteRuntime(_runtime);
    _runtime._0 = nullptr;
    _handle._0 = nullptr;
    validate(res);
  }

  AUTD3_API
  [[nodiscard]] coro::task<std::vector<std::optional<driver::FPGAState>>> fpga_state_async() {
    const size_t num_devices = geometry().num_devices();
    auto future = AUTDControllerFPGAState(_ptr);
    auto result = co_await wait_future(_handle, std::move(future));
    const auto p = validate(result);
    std::vector<std::optional<driver::FPGAState>> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      const auto state = AUTDControllerFPGAStateGet(p, i);
      ret.emplace_back(state < 0 ? std::nullopt : std::optional(driver::FPGAState(static_cast<uint8_t>(state))));
    }
    AUTDControllerFPGAStateDelete(p);
    co_return ret;
  }

  template <driver::datagram D>
  AUTD3_API [[nodiscard]] coro::task<void> send_async(const D& d) {
    auto future = AUTDControllerSend(_ptr, d.ptr(geometry()));
    auto result = co_await wait_future(_handle, std::move(future));
    validate(result);
  }

  AUTD3_API [[nodiscard]] coro::task<std::vector<driver::FirmwareVersion>> firmware_version_async() {
    const size_t num_devices = geometry().num_devices();
    auto future = AUTDControllerFirmwareVersionListPointer(_ptr);
    auto result = co_await wait_future(_handle, std::move(future));
    const auto handle = validate(result);
    std::vector<driver::FirmwareVersion> ret;
    ret.reserve(num_devices);
    for (uint32_t i = 0; i < static_cast<uint32_t>(num_devices); i++) {
      char info[256]{};
      AUTDControllerFirmwareVersionGet(handle, i, info);
      ret.emplace_back(std::string(info));
    }
    AUTDControllerFirmwareVersionListPointerDelete(handle);
    co_return ret;
  }
#endif

 private:
  AUTD3_API Controller(const native_methods::GeometryPtr geometry, const native_methods::RuntimePtr runtime, const native_methods::HandlePtr handle,
                       const native_methods::ControllerPtr ptr, L link)
      : Geometry(geometry), _runtime(runtime), _handle(handle), _ptr(ptr), _link(std::move(link)) {}

  native_methods::RuntimePtr _runtime;
  native_methods::HandlePtr _handle;
  native_methods::ControllerPtr _ptr;
  L _link;
};

}  // namespace autd3::controller
