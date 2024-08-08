#pragma once

#include "../user_data_repository.hpp"

namespace repository {

class YandexLoginDataProvider : public UserDataRepository {
 public:
  explicit YandexLoginDataProvider(userver::clients::http::Client& http_client);

  std::optional<AuthData> GetUserData(const std::string& token) override;
  std::optional<AuthData> GetUserDataByLogin(const std::string& login) override;
  void SaveUserData(const repository::AuthData&) override;
  std::vector<AuthData> GetUserDataByIds(
      const std::vector<int64_t>& id) override;

      private : userver::clients::http::Client& http_client_;
};

}  // namespace repository
