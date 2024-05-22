#include <userver/utest/utest.hpp>

#include <memory>

#include "../repository/user_data_repository.hpp"
#include "../services/auth/yandex_auth.hpp"
#include "../services/auth_service.hpp"
#include "mock_repositories.hpp"

namespace {

const repository::AuthData kRepositoryUser{
    .user_id = 123,
    .token = "token",
    .login = "login",
    .name = "name",
    .phone = "phone",
    .permission_group = {},
};

const services::AuthData kExpectedServiceUser{
    .user_id = 123,
    .login = "login",
    .name = "name",
    .permissions = {},
};

}  // namespace

UTEST(AuthService, DbAuth) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto db_repository = std::make_unique<MockUserDataRepository>();
  db_repository->SetResponse(kRepositoryUser);

  mock_repository_factory->SetUserDataDbRepository(std::move(db_repository));

  auto service = std::make_unique<services::YandexAuthService>(
      std::move(mock_repository_factory));

  auto result = service->GetAuthDataByToken("");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value(), kExpectedServiceUser);
}

UTEST(AuthService, HttpAuth) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto db_repository = std::make_unique<MockUserDataRepository>();

  mock_repository_factory->SetUserDataDbRepository(std::move(db_repository));

  auto http_repository = std::make_unique<MockUserDataRepository>();
  http_repository->SetResponse(kRepositoryUser);

  mock_repository_factory->SetUserDataHttpRepository(
      std::move(http_repository));

  auto service = std::make_unique<services::YandexAuthService>(
      std::move(mock_repository_factory));

  auto result = service->GetAuthDataByToken("");
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value(), kExpectedServiceUser);
}

UTEST(AuthService, UserNotFound) {
  auto mock_repository_factory = std::make_unique<MockRepositoryFactory>();

  auto db_repository = std::make_unique<MockUserDataRepository>();

  mock_repository_factory->SetUserDataDbRepository(std::move(db_repository));

  auto http_repository = std::make_unique<MockUserDataRepository>();
  mock_repository_factory->SetUserDataHttpRepository(
      std::move(http_repository));

  auto service = std::make_unique<services::YandexAuthService>(
      std::move(mock_repository_factory));

  auto result = service->GetAuthDataByToken("");
  ASSERT_FALSE(result.has_value());
}
