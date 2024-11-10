#pragma once

#include <string>
#include <vector>

#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/storages/postgres/io/type_mapping.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Attachment {
  boost::uuids::uuid id;
  std::string filename;

  bool operator<(const Attachment& other) const noexcept {
    if (id < other.id) return true;
    if (id > other.id) return false;
    return filename < other.filename;
  }
};

struct Request {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  std::vector<Attachment> attachments;
  std::string status;
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
  std::string status;
  std::vector<RequestComment> comments;
  std::vector<Attachment> attachments;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
};

struct RequestShort {
  boost::uuids::uuid request_id{};
  int64_t author_id{};
  userver::storages::postgres::TimePointTz created_at{};
};

struct RequestFilters {
  std::optional<boost::uuids::uuid> request_id;
  std::optional<int64_t> author_id;
  std::optional<boost::uuids::uuid> event_id;
  std::optional<userver::storages::postgres::TimePointTz> created_after{};
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
  virtual std::vector<RequestShort> GetFiltered(
      const RequestFilters& filters) = 0;
  virtual std::vector<RequestShort> GetAll() = 0;
};

}  // namespace repository

template <>
struct userver::storages::postgres::io::CppToUserPg<repository::Attachment> {
  static constexpr DBTypeName postgres_name = "service.attachment_v1";
};
