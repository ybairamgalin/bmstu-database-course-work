#include "simple_idm_service.hpp"

#include "services/exception.hpp"

namespace {

std::string ToString(services::UserRole role) {
  switch (role) {
    case services::UserRole::kUser:
      return "user";
    case services::UserRole::kModerator:
      return "moderator";
    case services::UserRole::kAdmin:
      return "admin";
  }

  throw std::runtime_error("Unknown role");
}

}  // namespace

namespace services {

SimpleIdmService::SimpleIdmService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : user_data_repository_(repository_factory->MakeUserDataDbRepository()),
      permission_repository_(repository_factory->MakePermissionRepository()) {}

void SimpleIdmService::HandleIdmRequest(IdmRequest&& request) {
  if (request.request_author.role != AuthRole::kAdmin) {
    throw ServiceLevelException("You cannot manage users permissions");
  }

  const auto user_data =
      user_data_repository_->GetUserDataByLogin(request.login);
  if (!user_data.has_value()) {
    throw ServiceLevelException(
        fmt::format("User with login {} does not exist", request.login));
  }

  if (!request.permission_groups.empty()) {
    const auto db_permission_groups =
        permission_repository_->GetPermissionByNames(request.permission_groups);
    if (db_permission_groups.size() != request.permission_groups.size()) {
      throw ServiceLevelException("Some permission groups are missing",
                                  ErrorType::kNotFound);
    }
  }

  user_data_repository_->AddUserToPermissionGroups(user_data->user_id,
                                                   request.permission_groups);

  user_data_repository_->SerUserRole(request.login,
                                     ::ToString(request.target_role));
}

}  // namespace services
