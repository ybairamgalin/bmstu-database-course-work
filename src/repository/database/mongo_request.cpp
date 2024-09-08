#include "mongo_request.hpp"

#include <utility>

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/bson.hpp>
#include <userver/formats/bson/value_builder.hpp>
#include <userver/storages/mongo.hpp>
#include <userver/storages/postgres/io/time_of_day.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <utils/uuid.hpp>

using userver::formats::bson::MakeArray;
using userver::formats::bson::MakeDoc;

namespace repository {

MongoRequestRepository::MongoRequestRepository(
    userver::storages::mongo::PoolPtr pool)
    : pool_(std::move(pool)) {}

std::optional<RequestFull> MongoRequestRepository::GetRequestById(
    const boost::uuids::uuid& uuid) {
  auto mongo_requests = pool_->GetCollection("requests");
  auto result =
      mongo_requests.FindOne(MakeDoc("id", boost::uuids::to_string(uuid)));

  if (!result) {
    return std::nullopt;
  }

  RequestFull request_full{};
  request_full.request_id =
      utils::ToUuid(result.value()["id"].As<std::string>()).value();
  request_full.event_id =
      utils::ToUuid(result.value()["event_id"].As<std::string>()).value();
  request_full.author_id = result.value()["author_id"].As<int64_t>();
  request_full.description = result.value()["description"].As<std::string>();
  request_full.status = result.value()["status"].As<std::string>();

  for (const auto& comment : result.value()["comments"]) {
    RequestComment request_comment;
    request_comment.content = comment["content"].As<std::string>();
    request_comment.author_id = comment["author_id"].As<int64_t>();
    request_comment.created_at =
        comment["created_at"].As<userver::storages::postgres::TimePointTz>();
    request_full.comments.push_back(request_comment);
  }

  for (const auto& attachment : result.value()["attachments"]) {
    Attachment request_attachment;
    request_attachment.id =
        utils::ToUuid(attachment["id"].As<std::string>()).value();
    request_attachment.filename = attachment["filename"].As<std::string>();
    request_full.attachments.push_back(request_attachment);
  }

  request_full.created_at = result.value()["created_at"]
                                .As<userver::storages::postgres::TimePointTz>();
  request_full.updated_at = result.value()["updated_at"]
                                .As<userver::storages::postgres::TimePointTz>();

  return request_full;
}

std::vector<RequestShort> MongoRequestRepository::GetAll() {
  auto mongo_requests = pool_->GetCollection("requests");
  auto cursor = mongo_requests.Find(MakeDoc());

  std::vector<RequestShort> requests;
  for (const auto& doc : cursor) {
    RequestShort request_short;
    request_short.request_id =
        utils::ToUuid(doc["id"].As<std::string>()).value();
    request_short.author_id = doc["author_id"].As<int64_t>();
    request_short.created_at =
        doc["created_at"].As<userver::storages::postgres::TimePointTz>();
    requests.push_back(request_short);
  }

  return requests;
}

void MongoRequestRepository::Insert(const Request& request) {
  auto mongo_requests = pool_->GetCollection("requests");

  auto now = userver::utils::datetime::Now();

  auto attachments = userver::formats::bson::ValueBuilder(
      userver::formats::common::Type::kArray);
  for (const auto& attachment : request.attachments) {
    attachments.PushBack(MakeDoc("id", boost::uuids::to_string(attachment.id),
                                 "filename", attachment.filename));
  }

  mongo_requests.InsertOne(MakeDoc(
      "id", boost::uuids::to_string(request.request_id), "description",
      request.description, "status", "new", "author_id", request.author_id,
      "event_id", boost::uuids::to_string(request.event_id), "created_at", now,
      "updated_at", now, "attachments", attachments.ExtractValue(), "comments",
      MakeArray()));
}

void MongoRequestRepository::Update(const Request& request) {
  auto mongo_requests = pool_->GetCollection("requests");

  auto attachments = userver::formats::bson::ValueBuilder(
      userver::formats::common::Type::kArray);
  for (const auto& attachment : request.attachments) {
    attachments.PushBack(MakeDoc("id", boost::uuids::to_string(attachment.id),
                                 "filename", attachment.filename));
  }

  auto now = userver::utils::datetime::Now();

  mongo_requests.UpdateOne(
      MakeDoc("id", boost::uuids::to_string(request.request_id)),
      MakeDoc(
          "$set",
          MakeDoc("description", request.description, "status", request.status,
                  "author_id", request.author_id, "event_id",
                  boost::uuids::to_string(request.event_id), "updated_at", now,
                  "attachments", attachments.ExtractValue())));
}

void MongoRequestRepository::AddComment(const boost::uuids::uuid& id,
                                        const std::string& content,
                                        int64_t author_id) {
  auto mongo_requests = pool_->GetCollection("requests");

  RequestComment comment;
  comment.content = content;
  comment.author_id = author_id;
  comment.created_at = userver::storages::postgres::Now();

  mongo_requests.UpdateOne(
      MakeDoc("id", boost::uuids::to_string(id)),
      MakeDoc("$push",
              MakeDoc("comments", MakeDoc("content", comment.content,
                                          "author_id", comment.author_id,
                                          "created_at", comment.created_at))));
}

}  // namespace repository
