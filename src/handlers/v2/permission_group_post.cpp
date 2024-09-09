#include "permission_group_post.hpp"

namespace handlers::v2 {

PermissionGroupPost::PermissionGroupPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

PermissionGroupPost::Response PermissionGroupPost::HandleJson(
    http::RequestWithBody<gen::PermissionGroupPost>&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");

  services_->MakePermissionService()->AddPermissionGroup(
      services::PermissionGroup{request.body.name, request.body.description},
      auth);
  return {201, {}};
}
}  // namespace handlers::v2
