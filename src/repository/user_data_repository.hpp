#pragma once

#include <memory>
#include <string>

#include <userver/clients/http/client.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "../services/auth_service.hpp"

namespace repository {

struct AuthData {
  int64_t user_id;
  std::string token;
  std::string login;
  std::string name;
  std::string phone;
  std::vector<std::string> permission_group;
};

class UserDataRepository {
 public:
  virtual ~UserDataRepository() = default;
  virtual std::optional<AuthData> GetUserData(const std::string& token) = 0;
  virtual void SaveUserData(const AuthData&) = 0;
};

class UserDataRepositoryFactory {
 public:
  virtual ~UserDataRepositoryFactory() = default;
  virtual std::unique_ptr<UserDataRepository> GetDbRepository() = 0;
  virtual std::unique_ptr<UserDataRepository> GetHttpRepository() = 0;
};

class YandexDataRepositoryFactory : public UserDataRepositoryFactory {
 public:
  YandexDataRepositoryFactory(
      userver::clients::http::Client& http_client,
      userver::storages::postgres::ClusterPtr cluster_ptr);
  std::unique_ptr<UserDataRepository> GetDbRepository() override;
  std::unique_ptr<UserDataRepository> GetHttpRepository() override;

 private:
  userver::clients::http::Client& http_client_;
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
