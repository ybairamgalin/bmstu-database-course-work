#include "permission.hpp"

namespace repository {

PgPermissionRepository::PgPermissionRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void PgPermissionRepository::AddPermission(const DbPermission& permission) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.permissions (name, description) "
                        "values ($1, $2) ",
                        permission.name, permission.description);
}

std::vector<DbPermission> PgPermissionRepository::GetPermissionByNames(
    const std::vector<std::string>& names) {
  return cluster_ptr_
      ->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                "select name, description "
                "from service.permissions "
                "where name in (select unnest($1))",
                names)
      .AsContainer<std::vector<DbPermission>>(
          userver::storages::postgres::kRowTag);
}

}  // namespace repository
