#pragma once

#include <memory>

#include "../../repository/factory.hpp"
#include "../request_management_service.hpp"

namespace services {

class RequestManagementService : public IRequestManagementService {
 public:
  explicit RequestManagementService(
      const std::shared_ptr<repository::IRepositoryFactory>&
          repository_factory);
  void AddRequest(const services::Request& request) override;
  void UpdateRequest(const services::Request& request) override;
 private:
  std::unique_ptr<repository::RequestsRepository> request_repository_;
};

}  // namespace services
