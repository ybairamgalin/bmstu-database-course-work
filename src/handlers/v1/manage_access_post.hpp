#pragma once

#include "base_handler.hpp"

namespace handlers {

class ManageAccessPost : public BaseHandlerWithAuth {
 public:
  static constexpr std::string_view kName = "manage_access_post";

  ManageAccessPost(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context);

  userver::formats::json::Value Handle(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      const services::AuthData& user_data) const override;
};

}  // namespace handlers
