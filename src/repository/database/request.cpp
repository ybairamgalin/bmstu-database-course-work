#include "request.hpp"

namespace {

struct DbRequest {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
};

}  // namespace

namespace repository {

DbRequestsRepository::DbRequestsRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

std::optional<RequestFull> DbRequestsRepository::GetRequestById(
    const boost::uuids::uuid& uuid) {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select request_id, event_id, user_id, "
      "    description, created_at, updated_at "
      "from service.requests "
      "where request_id = $1",
      uuid);
  if (result.IsEmpty()) {
    return std::nullopt;
  }

  auto db_request =
      result.AsSingleRow<DbRequest>(userver::storages::postgres::kRowTag);

  auto comments =
      cluster_ptr_
          ->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                    "select content, author_id, created_at "
                    "from service.comments "
                    "where request_id = $1 "
                    "order by created_at",
                    uuid)
          .AsContainer<std::vector<RequestComment>>(
              userver::storages::postgres::kRowTag);

  return RequestFull{db_request.request_id, db_request.event_id,
                     db_request.author_id,  db_request.description,
                     std::move(comments),   db_request.created_at,
                     db_request.updated_at};
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
void DbRequestsRepository::AddComment(const boost::uuids::uuid& id,
                                      const std::string& content,
                                      int64_t author_id) {
  cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "insert into service.comments (request_id, author_id, content) "
      "values ($1, $2, $3)",
      id, author_id, content);
}

}  // namespace repository
