#pragma once

#include "../user_data_repository.hpp"

namespace repository {

class YandexLoginDataProvider : public UserDataRepository {
 public:
  explicit YandexLoginDataProvider(userver::clients::http::Client& http_client);

  std::optional<AuthData> GetUserData(const std::string& token) override;
  void SaveUserData(const repository::AuthData&) override;

 private:
  userver::clients::http::Client& http_client_;
};

}  // namespace repository
