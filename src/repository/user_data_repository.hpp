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
  std::string role;
  std::set<std::string> permission_group;
};

class UserDataRepository {
 public:
  virtual ~UserDataRepository() = default;
  virtual std::optional<AuthData> GetUserData(const std::string& token) = 0;
  virtual std::optional<AuthData> GetUserDataByLogin(
      const std::string& login) = 0;
  virtual void SaveUserData(const AuthData&) = 0;
};

}  // namespace repository
