#include "request.hpp"

namespace repository {

DbRequestsRepository::DbRequestsRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void DbRequestsRepository::Insert(const repository::Request& request) {
  cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "insert into service.requests (event_id, user_id, description) "
      "values ($1, $2, $3) "
      "on conflict (request_id) "
      "do update set "
      "description = excluded.description ",
      request.event_id, request.author_id, request.description);
}

void DbRequestsRepository::Update(const repository::Request& request) {
  cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "update service.requests set "
      "description = $2 "
      "event_id = $3 "
      "where request_id = $1 "
      "description = excluded.description ",
      request.request_id, request.description, request.event_id);
}

}  // namespace repository
