#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Nop {
  Nop() = default;  // LCOV_EXCL_LINE

 public:
  class Builder final {
    friend class Nop;
    friend class controller::ControllerBuilder;

    AUTD3_API Builder() = default;  // LCOV_EXCL_LINE

    AUTD3_API [[nodiscard]] static Nop resolve_link(native_methods::HandlePtr, const native_methods::LinkPtr) { return {}; }

   public:
    using Link = Nop;

    AUTD3_API [[nodiscard]] static native_methods::LinkBuilderPtr ptr() { return native_methods::AUTDLinkNop(); }
  };

  static Builder builder() { return {}; }
};
}  // namespace autd3::link
