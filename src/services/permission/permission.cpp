#include "permission.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "services/exception.hpp"

namespace services {

PermissionService::PermissionService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : permission_repository_(repository_factory->MakePermissionRepository()) {}

void PermissionService::AddPermissionGroup(const PermissionGroup& permission,
                                           const AuthData& auth) {
  if (auth.role != AuthRole::kAdmin) {
    throw ServiceLevelException("Cannot add permissions",
                                ErrorType::kPermissionDenied);
  }
  const auto db_permissions =
      permission_repository_->GetPermissionByNames({permission.name});
  if (!db_permissions.empty()) {
    throw ServiceLevelException("Permission already exists",
                                ErrorType::kConflict);
  }

  permission_repository_->AddPermission(
      repository::DbPermission{permission.name, permission.description});
}
}  // namespace services
