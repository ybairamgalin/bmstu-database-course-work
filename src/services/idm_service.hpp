#pragma once

#include <string>

#include "models/permission.hpp"

namespace services {

// enum class ActionType {
//   kGrantAccess,
//   kRevokeAccess,
// };
//
// struct IdmRequest {
//   ActionType action_type;
//   std::string login;
//   Permission permission;
// };

enum class UserRole { kUser, kModerator, kAdmin };

struct IdmRequest {
  std::string login;
  UserRole target_role;
};

class IIdmService {
 public:
  virtual ~IIdmService() = default;
  virtual void HandleIdmRequest(IdmRequest&& request) = 0;
};

}  // namespace services
