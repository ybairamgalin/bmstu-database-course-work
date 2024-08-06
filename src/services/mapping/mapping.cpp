#include "mapping.hpp"

namespace services::internal {

services::AuthData MapAuthData(const repository::AuthData& auth_data) {
  std::set<services::Permission> permissions;
  for (const auto& permission : auth_data.permission_group) {
    if (permission == "moderator") {
      permissions.emplace(services::Permission::kModerateRequests);
    } else if (permission == "admin") {
      permissions.emplace(services::Permission::kManageModerators);
    }
  }

  services::AuthData result;
  result.user_id = auth_data.user_id;
  result.login = auth_data.login;
  result.name = auth_data.name;
  result.permissions = permissions;

  if (auth_data.role == "admin") {
    result.role = AuthRole::kAdmin;
  } else if (auth_data.role == "moderator") {
    result.role = AuthRole::kModerator;
  } else {
    result.role = AuthRole::kUser;
  }

  return result;
}

}  // namespace services::internal
