#pragma once

#include <memory>

#include <userver/storages/postgres/cluster.hpp>

#include "../../repository/factory.hpp"
#include "../../repository/user_data_repository.hpp"
#include "../idm_service.hpp"

namespace services {

class IdmService : public IIdmService {
 public:
  explicit IdmService(const std::shared_ptr<repository::IRepositoryFactory>&
                          repository_factory);
  void HandleIdmRequest(services::IdmRequest&& request) override;

 private:
  std::unique_ptr<repository::UserDataRepository> user_data_repository_;
};

}  // namespace services
