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

  return services::AuthData{auth_data.user_id, auth_data.login, auth_data.name,
                            permissions};
}

}  // namespace services::internal
