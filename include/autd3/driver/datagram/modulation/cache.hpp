#pragma once

#include <memory>

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
class Cache final : public driver::ModulationBase<Cache<M>> {
 public:
  AUTD3_API explicit Cache(M m) : _m(std::move(m)), _ptr(nullptr) { this->_loop_behavior = _m.loop_behavior(); }
  Cache(const Cache& v) = default;  // LCOV_EXCL_LINE
  Cache& operator=(const Cache& obj) = delete;
  Cache(Cache&& obj) noexcept = default;  // LCOV_EXCL_LINE
  Cache& operator=(Cache&& obj) noexcept = delete;
  ~Cache() noexcept override = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    if (!_ptr) {
      _ptr = std::shared_ptr<void>(const_cast<void*>(native_methods::AUTDModulationCache(_m.modulation_ptr())._0), [](const void* ptr) {
        if (!ptr) return;
        AUTDModulationCacheFree(native_methods::ModulationCachePtr{ptr});
      });
    }
    return AUTDModulationCacheClone(native_methods::ModulationCachePtr{_ptr.get()}, this->_loop_behavior);
  }

  M _m;
  mutable std::shared_ptr<void> _ptr;
};

}  // namespace autd3::modulation

namespace autd3::driver {

template <class M>
class IntoModulationCache {
 public:
  AUTD3_API [[nodiscard]] modulation::Cache<M> with_cache() & { return modulation::Cache(*static_cast<M*>(this)); }  // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] modulation::Cache<M> with_cache() && { return modulation::Cache(std::move(*static_cast<M*>(this))); }
};

}  // namespace autd3::driver
