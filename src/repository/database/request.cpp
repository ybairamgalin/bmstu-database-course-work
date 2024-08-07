#include "request.hpp"

namespace repository {

DbRequestsRepository::DbRequestsRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

std::vector<RequestFull> DbRequestsRepository::GetRequestsByIds(
    const std::vector<boost::uuids::uuid>& uuids) {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select request_id, event_id, user_id, "
      "    description, created_at, updated_at "
      "from service.requests "
      "where request_id in (select unnest($1))",
      uuids);

  return result.AsContainer<std::vector<RequestFull>>(
      userver::storages::postgres::kRowTag);
}

void DbRequestsRepository::Insert(const repository::Request& request) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.requests (request_id, event_id, "
                        "user_id, description) "
                        "values ($1, $2, $3, $4)",
                        request.request_id, request.event_id, request.author_id,
                        request.description);
}

void DbRequestsRepository::Update(const repository::Request& request) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "update service.requests set "
                        "description = $2 "
                        "event_id = $3 "
                        "where request_id = $1 "
                        "description = excluded.description ",
                        request.request_id, request.description,
                        request.event_id);
}

}  // namespace repository
