#pragma once

#include <string>

namespace services {

struct PermissionGroup {
  std::string name;
  std::string description;
};

class IPermissionService {
 public:
  virtual ~IPermissionService() = default;
  virtual void AddPermissionGroup(const PermissionGroup& permission,
                                  const AuthData& auth) = 0;
};

}  // namespace services
