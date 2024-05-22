#pragma once

#include <optional>
#include <set>
#include <string>

#include "models/permission.hpp"

namespace services {

struct AuthData {
  int64_t user_id;
  std::string login;
  std::string name;
  std::set<Permission> permissions;

  bool operator==(const AuthData& other) const {
    return user_id == other.user_id && login == other.login &&
           name == other.name && permissions == other.permissions;
  }
};

class IAuthService {
 public:
  virtual ~IAuthService() = default;
  virtual std::optional<AuthData> GetAuthDataByToken(
      const std::string& token) = 0;
};

}  // namespace services
