#include "roles.hpp"

namespace {

services::UserRole MapRole(gen::ManageAccessPutBody::New_Role role) {
  switch (role) {
    case gen::ManageAccessPutBody::New_Role::kUser:
      return services::UserRole::kUser;
    case gen::ManageAccessPutBody::New_Role::kModerator:
      return services::UserRole::kModerator;
    case gen::ManageAccessPutBody::New_Role::kAdmin:
      return services::UserRole::kAdmin;
  }
  throw std::runtime_error("Unknown role");
}

}  // namespace

namespace handlers::v3 {

RolesPut::RolesPut(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RolesPut::Response RolesPut::HandleJson(
    RolesPut::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");

  const auto new_role = request.body.new_role.has_value()
                            ? MapRole(request.body.new_role.value())
                            : services::UserRole::kUser;

  services_->MakeIdmService()->HandleIdmRequest(
      services::IdmRequest{request.body.login, new_role, auth,
                           request.body.permission_groups.has_value()
                               ? request.body.permission_groups.value()
                               : std::vector<std::string>()});
  return {200, {}};
}

}  // namespace handlers::v3
