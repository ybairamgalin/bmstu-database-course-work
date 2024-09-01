#include "manage_access_put.hpp"

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

namespace handlers::v2 {

ManageAccessPut::ManageAccessPut(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

ManageAccessPut::Response ManageAccessPut::HandleJson(
    ManageAccessPut::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");

  services_->MakeIdmService()->HandleIdmRequest(services::IdmRequest{
      request.body.login, MapRole(request.body.new_role), auth});
  return {200, {}};
}

}  // namespace handlers::v2
