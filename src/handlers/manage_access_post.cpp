#include "manage_access_post.hpp"

#include "../services/idm/idm.hpp"

namespace handlers {

ManageAccessPost::ManageAccessPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : BaseHandlerWithAuth(config, component_context) {}

userver::formats::json::Value ManageAccessPost::Handle(
    const userver::server::http::HttpRequest&,
    const userver::formats::json::Value& request_json,
    const services::AuthData& user_data) const {
  userver::formats::json::ValueBuilder builder;
  if (!user_data.permissions.count(services::Permission::kManageModerators)) {
    builder["message"] = "Not enough permissions";
    return builder.ExtractValue();
  }
  if (!request_json.HasMember("action") || !request_json.HasMember("login") ||
      !request_json.HasMember("permission_slug") ||
      !request_json["action"].IsString() || !request_json["login"].IsString() ||
      !request_json["permission_slug"].IsString()) {
    builder["message"] =
        "Action, user_login and permission_slug should be provided";
    return builder.ExtractValue();
  }
  services::IdmRequest idm_request{};
  if (request_json["action"].As<std::string>() == "grant") {
    idm_request.action_type = services::ActionType::kGrantAccess;
  } else if (request_json["action"].As<std::string>() == "revoke") {
    idm_request.action_type = services::ActionType::kRevokeAccess;
  } else {
    builder["message"] = "Unknown action";
    return builder.ExtractValue();
  }

  if (request_json["permission_slug"].As<std::string>() == "admin") {
    idm_request.permission = services::Permission::kManageModerators;
  } else if (request_json["permission_slug"].As<std::string>() == "moderator") {
    idm_request.permission = services::Permission::kModerateRequests;
  } else {
    builder["message"] = "Unknown permission_slug";
    return builder.ExtractValue();
  }

  idm_request.login = request_json["login"].As<std::string>();

  services::IdmService(cluster_ptr_).HandleIdmRequest(std::move(idm_request));
  builder["message"] = "Ok";
  return builder.ExtractValue();
}

}  // namespace handlers
