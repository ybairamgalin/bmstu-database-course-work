#pragma once

#include <string>
#include <vector>

#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Request {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  std::vector<boost::uuids::uuid> attachment_ids;
};

struct RequestFull {
  boost::uuids::uuid request_id;
  boost::uuids::uuid event_id;
  int64_t author_id;
  std::string description;
  //  std::vector<boost::uuids::uuid> attachment_ids;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
};

class RequestsRepository {
 public:
  virtual ~RequestsRepository() = default;
  virtual std::vector<RequestFull> GetRequestsByIds(
      const std::vector<boost::uuids::uuid>& uuids) = 0;
  //  virtual Request
  virtual void Insert(const Request& request) = 0;
  virtual void Update(const Request& request) = 0;
};

}  // namespace repository
