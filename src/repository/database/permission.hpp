#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include <repository/permission.hpp>

namespace repository {

class PgPermissionRepository : public PermissionRepository {
 public:
  explicit PgPermissionRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);

  std::vector<DbPermission> GetPermissionByNames(
      const std::vector<std::string>& names) override;
  void AddPermission(const repository::DbPermission& permission) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
