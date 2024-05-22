#include "idm.hpp"

#include <memory>

#include <userver/storages/postgres/cluster.hpp>

#include "../../repository/database/user_data.hpp"
#include "../../repository/user_data_repository.hpp"
#include "../exception.hpp"
#include "../mapping/mapping.hpp"

namespace services {

IdmService::IdmService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : user_data_repository_(repository_factory->MakeUserDataDbRepository()) {}

void IdmService::HandleIdmRequest(services::IdmRequest&& request) {
  auto db_user = user_data_repository_->GetUserDataByLogin(request.login);
  if (!db_user) {
    throw services::ServiceLevelException("User not found");
  }

  if (request.action_type == ActionType::kGrantAccess) {
    if (db_user->permission_group.count(ToString(request.permission))) {
      throw services::ServiceLevelException("Permission was already granted");
    }
    db_user->permission_group.emplace(ToString(request.permission));
  } else if (request.action_type == ActionType::kRevokeAccess) {
    if (!db_user->permission_group.count(ToString(request.permission))) {
      throw services::ServiceLevelException(
          "User does not have the requested permission");
    }
    db_user->permission_group.erase(ToString(request.permission));
  }
  user_data_repository_->SaveUserData(db_user.value());
}

}  // namespace services
