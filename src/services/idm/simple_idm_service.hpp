#pragma once

#include <memory>

#include "repository/factory.hpp"
#include "services/idm_service.hpp"

namespace services {

class SimpleIdmService : public IIdmService {
 public:
  explicit SimpleIdmService(
      const std::shared_ptr<repository::IRepositoryFactory>&
          repository_factory);

  void HandleIdmRequest(services::IdmRequest&& request) override;

 private:
  std::unique_ptr<repository::UserDataRepository> user_data_repository_;
};

}  // namespace services
