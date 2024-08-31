#pragma once

#include <optional>
#include <string>

#include <userver/clients/http/client.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "../../repository/factory.hpp"
#include "../../repository/user_data_repository.hpp"
#include "../auth_service.hpp"

namespace services {

class YandexAuthService : public services::IAuthService {
 public:
  explicit YandexAuthService(
      const std::shared_ptr<repository::IRepositoryFactory>& repository_factory);
  std::optional<services::AuthData> GetAuthDataByToken(
      const std::string& token) override;

 private:
  std::unique_ptr<repository::UserDataRepository> db_repository_;
  std::unique_ptr<repository::UserDataRepository> http_repository_;
};

}  // namespace services
