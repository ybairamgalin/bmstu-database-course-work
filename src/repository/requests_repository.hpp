#pragma once

#include <string>
#include <vector>

#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Attachment {
  boost::uuids::uuid id;
  std::string filename;
};

struct Request {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  std::vector<Attachment> attachments;
};

struct RequestComment {
  std::string content;
  int64_t author_id;
  userver::storages::postgres::TimePointTz created_at;
};

struct RequestFull {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  std::vector<RequestComment> comments;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
};

class RequestsRepository {
 public:
  virtual ~RequestsRepository() = default;
  virtual std::optional<RequestFull> GetRequestById(
      const boost::uuids::uuid& uuid) = 0;
  virtual void Insert(const Request& request) = 0;
  virtual void Update(const Request& request) = 0;
  virtual void AddComment(const boost::uuids::uuid& id,
                          const std::string& content, int64_t author_id) = 0;
};

}  // namespace repository
