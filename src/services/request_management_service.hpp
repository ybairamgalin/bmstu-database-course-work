
#pragma once

#include <optional>
#include <string>

#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid.hpp>

namespace services {

struct RequestToCreateOrUpdate {
  int author_id;
  boost::uuids::uuid event_id;
  std::string description;
  std::vector<boost::uuids::uuid> attachment_ids;
};

struct UserInfo {
  int64_t id;
  std::string name;
  std::string login;
  std::string phone;
};

struct RequestComment {
  std::string content;
  UserInfo author;
  userver::utils::datetime::TimePointTz created_at;
};

struct Request {
  boost::uuids::uuid event_id;
  UserInfo author;
  std::string description;
  std::vector<boost::uuids::uuid> attachment_ids;
  std::vector<RequestComment> comments;
  userver::utils::datetime::TimePointTz created_at;
  userver::utils::datetime::TimePointTz updated_at;
};

class IRequestManagementService {
 public:
  virtual ~IRequestManagementService() = default;
  virtual Request GetRequestById(const boost::uuids::uuid& request_id) = 0;
  virtual boost::uuids::uuid AddRequest(
      const RequestToCreateOrUpdate& request) = 0;
  virtual void AddComment(const boost::uuids::uuid& request_id,
                          const std::string& content, int64_t author_id) = 0;
  virtual void UpdateRequest(const boost::uuids::uuid& request_id,
                             const RequestToCreateOrUpdate& request) = 0;
};

}  // namespace services
