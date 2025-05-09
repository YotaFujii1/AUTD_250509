#pragma once

#include <chrono>
#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Simulator final {
 public:
  class Builder final {
    friend class Simulator;
    friend class controller::ControllerBuilder;

    AUTD3_API explicit Builder(std::string ip) : _ip(std::move(ip)) {}

    [[nodiscard]] static Simulator resolve_link(const native_methods::HandlePtr, const native_methods::LinkPtr) { return Simulator{}; }

   public:
    using Link = Simulator;

    AUTD3_DEF_PROP(std::string, ip)

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const {
      return native_methods::LinkBuilderPtr{validate(native_methods::AUTDLinkSimulator(_ip.c_str()))._0};
    }
  };

  AUTD3_API [[nodiscard]] static Builder builder(const std::string& ip) { return Builder(ip); }
};

}  // namespace autd3::link
