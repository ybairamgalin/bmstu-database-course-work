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
};

class IAuthService {
 public:
  virtual ~IAuthService() = default;
  virtual std::optional<AuthData> GetAuthDataByToken(
      const std::string& token) = 0;
};

}  // namespace services
