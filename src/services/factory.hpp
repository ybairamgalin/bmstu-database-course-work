#pragma once

#include <memory>

#include "userver/clients/http/client.hpp"
#include "userver/storages/postgres/cluster.hpp"

#include "auth_service.hpp"
#include "event_service.hpp"
#include "file_service.hpp"
#include "idm_service.hpp"
#include "repository/factory.hpp"
#include "request_management_service.hpp"
#include "permission_service.hpp"

namespace services {

class IServiceFactory {
 public:
  virtual ~IServiceFactory() = default;
  virtual std::unique_ptr<IIdmService> MakeIdmService() = 0;
  virtual std::unique_ptr<IAuthService> MakeAuthService() = 0;
  virtual std::unique_ptr<IRequestManagementService>
  MakeRequestManagementService() = 0;
  virtual std::unique_ptr<IEventService> MakeEventService() = 0;
  virtual std::unique_ptr<IFileService> MakeFileService() = 0;
};

class SimpleServiceFactory : public IServiceFactory {
 public:
  explicit SimpleServiceFactory(
      std::unique_ptr<repository::IRepositoryFactory> repository_factory);
  std::unique_ptr<IIdmService> MakeIdmService() override;
  std::unique_ptr<IAuthService> MakeAuthService() override;
  std::unique_ptr<IRequestManagementService> MakeRequestManagementService()
      override;
  std::unique_ptr<IEventService> MakeEventService() override;
  std::unique_ptr<IFileService> MakeFileService() override;

 private:
  std::shared_ptr<repository::IRepositoryFactory> repository_factory_;
};

}  // namespace services
