#pragma once

#include <chrono>
#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class TwinCAT final {
  TwinCAT() = default;  // LCOV_EXCL_LINE

 public:
  class Builder final {
    friend class TwinCAT;
    friend class controller::ControllerBuilder;

    Builder() = default;

    [[nodiscard]] static TwinCAT resolve_link(native_methods::HandlePtr, native_methods::LinkPtr) { return {}; }

   public:
    using Link = TwinCAT;

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const { return native_methods::AUTDLinkTwinCAT(); }
  };

  AUTD3_API [[nodiscard]] static Builder builder() { return {}; }
};

class RemoteTwinCAT final {
  RemoteTwinCAT() = default;  // LCOV_EXCL_LINE

 public:
  class Builder final {
    friend class RemoteTwinCAT;
    friend class controller::ControllerBuilder;

    AUTD3_API [[nodiscard]] explicit Builder(std::string server_ams_net_id) : _server_ams_net_id(std::move(server_ams_net_id)) {}

    [[nodiscard]] static RemoteTwinCAT resolve_link(native_methods::HandlePtr, native_methods::LinkPtr) { return {}; }

   public:
    using Link = RemoteTwinCAT;

    AUTD3_DEF_PROP(std::string, server_ams_net_id)
    AUTD3_DEF_PARAM(Builder, std::string, server_ip)
    AUTD3_DEF_PARAM(Builder, std::string, client_ams_net_id)

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const {
      return validate(native_methods::AUTDLinkRemoteTwinCAT(_server_ams_net_id.c_str(), _server_ip.c_str(), _client_ams_net_id.c_str()));
    }
  };

  AUTD3_API [[nodiscard]] static Builder builder(const std::string& server_ams_net_id) { return Builder(server_ams_net_id); }
};

}  // namespace autd3::link
