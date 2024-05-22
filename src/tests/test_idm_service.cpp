#include <userver/utest/utest.hpp>

#include "../services/exception.hpp"
#include "../services/idm/idm.hpp"
#include "../services/idm_service.hpp"
#include "mock_repositories.hpp"

UTEST(IdmService, GrantAccessOk) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto mock_user_data_repository = std::make_unique<MockUserDataRepository>();

  repository::AuthData user{};

  mock_user_data_repository->SetResponse(user);
  mock_repository_factory->SetUserDataDbRepository(
      std::move(mock_user_data_repository));

  services::IdmRequest request{services::ActionType::kGrantAccess, "login",
                               services::Permission::kManageModerators};

  auto service = std::make_unique<services::IdmService>(
      std::move(mock_repository_factory));

  service->HandleIdmRequest(std::move(request));
}

UTEST(IdmService, RevokeAccessOk) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto mock_user_data_repository = std::make_unique<MockUserDataRepository>();

  repository::AuthData user{};

  mock_user_data_repository->SetResponse(user);
  mock_repository_factory->SetUserDataDbRepository(
      std::move(mock_user_data_repository));

  services::IdmRequest request{services::ActionType::kGrantAccess, "login",
                               services::Permission::kManageModerators};

  auto service = std::make_unique<services::IdmService>(
      std::move(mock_repository_factory));

  service->HandleIdmRequest(std::move(request));
}

UTEST(IdmService, PermissionAlreadyRevoked) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto mock_user_data_repository = std::make_unique<MockUserDataRepository>();

  repository::AuthData user{};

  mock_user_data_repository->SetResponse(user);
  mock_repository_factory->SetUserDataDbRepository(
      std::move(mock_user_data_repository));

  services::IdmRequest request{services::ActionType::kRevokeAccess, "login",
                               services::Permission::kManageModerators};

  auto service = std::make_unique<services::IdmService>(
      std::move(mock_repository_factory));

  ASSERT_THROW(service->HandleIdmRequest(std::move(request)),
               services::ServiceLevelException);
}

UTEST(IdmService, PermissionAlreadyGranted) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto mock_user_data_repository = std::make_unique<MockUserDataRepository>();

  repository::AuthData user{};
  user.permission_group.emplace("admin");

  mock_user_data_repository->SetResponse(user);
  mock_repository_factory->SetUserDataDbRepository(
      std::move(mock_user_data_repository));

  services::IdmRequest request{services::ActionType::kGrantAccess, "login",
                               services::Permission::kManageModerators};

  auto service = std::make_unique<services::IdmService>(
      std::move(mock_repository_factory));

  ASSERT_THROW(service->HandleIdmRequest(std::move(request)),
               services::ServiceLevelException);
}

UTEST(IdmService, UserNotFound) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto service = std::make_unique<services::IdmService>(
      std::move(mock_repository_factory));

  ASSERT_THROW(service->HandleIdmRequest(services::IdmRequest{}),
               services::ServiceLevelException);
}
