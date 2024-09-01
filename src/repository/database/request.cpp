#include "request.hpp"

#include <boost/range/adaptor/transformed.hpp>

#include <userver/storages/postgres/typed_result_set.hpp>

#include "utils/merge_utils.hpp"

namespace b_ranges = boost::adaptors;

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

  auto attachments =
      cluster_ptr_
          ->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                    "select file_uuid, source_name "
                    "from service.request_file "
                    "where request_id = $1",
                    db_request.request_id)
          .AsContainer<std::vector<Attachment>>(
              userver::storages::postgres::kRowTag);

  return RequestFull{db_request.request_id, db_request.event_id,
                     db_request.author_id,  db_request.description,
                     std::move(comments),   std::move(attachments),
                     db_request.created_at, db_request.updated_at};
}

void DbRequestsRepository::Insert(const repository::Request& request) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.requests (request_id, event_id, "
                        "user_id, description) "
                        "values ($1, $2, $3, $4)",
                        request.request_id, request.event_id, request.author_id,
                        request.description);

  std::vector<boost::uuids::uuid> file_uuids;
  file_uuids.reserve(request.attachments.size());
  std::vector<std::string> file_names;
  file_names.reserve(request.attachments.size());
  for (const auto& attachment : request.attachments) {
    file_uuids.emplace_back(attachment.id);
    file_names.emplace_back(attachment.filename);
  }

  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.request_file "
                        "(request_id, file_uuid, source_name) "
                        "(select $1, unnest($2), unnest($3))",
                        request.request_id, file_uuids, file_names);
}

void DbRequestsRepository::Update(const repository::Request& request) {
  auto trx = cluster_ptr_->Begin(
      userver::storages::postgres::ClusterHostType::kMaster, {});

  trx.Execute(
      "update service.requests set "
      "description = $2, "
      "event_id = $3 "
      "where request_id = $1 ",
      request.request_id, request.description, request.event_id);

  auto attachments = trx.Execute(
                            "select file_uuid, source_name "
                            "from service.request_file "
                            "where request_id = $1",
                            request.request_id)
                         .AsContainer<std::set<Attachment>>(
                             userver::storages::postgres::kRowTag);
  auto merge_result = utils::Merge(
      attachments,
      std::set(request.attachments.begin(), request.attachments.end()));

  if (!merge_result.deleted.empty()) {
    trx.Execute(
        "delete from service.request_file "
        "where (request_id, file_uuid, source_name) "
        "in (select $2, id, filename from unnest($1::service.attachment_v1[]))",
        merge_result.deleted, request.request_id);
  }

  if (!merge_result.added.empty()) {
    trx.Execute(
        "insert into service.request_file ("
        "    request_id, file_uuid, source_name) "
        "select $2, id, filename from unnest($1::service.attachment_v1[])",
        merge_result.added, request.request_id);
  }

  trx.Commit();
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

std::vector<RequestShort> DbRequestsRepository::GetAll() {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kSlave,
      "select request_id, user_id, created_at "
      "from service.requests "
      "order by created_at desc");
  return result.AsContainer<std::vector<RequestShort>>(
      userver::storages::postgres::kRowTag);
}

}  // namespace repository
