#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "../requests_repository.hpp"

namespace repository {

class DbRequestsRepository : public RequestsRepository {
 public:
  explicit DbRequestsRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);
  void Insert(const repository::Request& request) override;
  void Update(const repository::Request& request) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
