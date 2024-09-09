#pragma once

#include <boost/uuid/uuid.hpp>
#include <optional>
#include <string>
#include <vector>

namespace repository {

struct DbPermission {
  std::string name;
  std::string description;
};

class PermissionRepository {
 public:
  virtual ~PermissionRepository() = default;
  virtual std::vector<DbPermission> GetPermissionByNames(
      const std::vector<std::string>& names) = 0;
  virtual void AddPermission(const DbPermission& permission) = 0;
};

}  // namespace repository
