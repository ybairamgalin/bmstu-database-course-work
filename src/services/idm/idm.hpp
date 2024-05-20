#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "../idm_service.hpp"

namespace services {

class IdmService : public IIdmService {
 public:
  explicit IdmService(userver::storages::postgres::ClusterPtr cluster_ptr);
  void HandleIdmRequest(services::IdmRequest&& request) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace services
