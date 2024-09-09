#pragma once

#include <repository/factory.hpp>
#include <repository/permission.hpp>
#include <services/permission_service.hpp>

namespace services {

class PermissionService : public IPermissionService {
 public:
  explicit PermissionService(
      const std::shared_ptr<repository::IRepositoryFactory>&
          repository_factory);

  void AddPermissionGroup(const services::PermissionGroup& permission,
                          const services::AuthData& auth) override;

 private:
  std::unique_ptr<repository::PermissionRepository> permission_repository_;
};

}  // namespace services
