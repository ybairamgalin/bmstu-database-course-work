
#pragma once

#include <optional>
#include <string>
#include <vector>

#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid.hpp>

#include "models/user.hpp"

namespace services {

struct Attachment {
  boost::uuids::uuid id;
  std::string filename;
};

struct RequestToCreateOrUpdate {
  AuthData author;
  boost::uuids::uuid event_id;
  std::string description;
  std::vector<Attachment> attachments;
  std::string status;
};

class RequestToCreateOrUpdateBuilder {
 public:
  RequestToCreateOrUpdateBuilder();

  RequestToCreateOrUpdateBuilder& WithAuthor(const AuthData& auth_data);
  RequestToCreateOrUpdateBuilder& WithEvent(const boost::uuids::uuid& event_id);
  RequestToCreateOrUpdateBuilder& WithDescription(const std::string& description);
  RequestToCreateOrUpdateBuilder& WithStatus(const std::string& status);

  RequestToCreateOrUpdate Build();

 private:
  RequestToCreateOrUpdate request_;
};

struct RequestComment {
  std::string content;
  UserInfo author;
  userver::utils::datetime::TimePointTz created_at;
};

struct AttachmentForDownload {
  std::string download_url;
  std::string filename;
};

struct Request {
  boost::uuids::uuid event_id;
  UserInfo author;
  std::string status;
  std::string description;
  std::vector<AttachmentForDownload> attachments;
  std::vector<RequestComment> comments;
  userver::utils::datetime::TimePointTz created_at;
  userver::utils::datetime::TimePointTz updated_at;
};

struct RequestShort {
  boost::uuids::uuid id;
  UserInfo author;
  userver::utils::datetime::TimePointTz created_at;
};

class IRequestManagementService {
 public:
  virtual ~IRequestManagementService() = default;
  virtual Request GetRequestById(const boost::uuids::uuid& request_id,
                                 const AuthData& auth) = 0;
  virtual boost::uuids::uuid AddRequest(
      const RequestToCreateOrUpdate& request) = 0;
  virtual void AddComment(const boost::uuids::uuid& request_id,
                          const std::string& content, const AuthData& auth) = 0;
  virtual void UpdateRequest(const boost::uuids::uuid& request_id,
                             const RequestToCreateOrUpdate& request) = 0;
  virtual std::vector<RequestShort> GetAll(const AuthData& auth) = 0;
};

}  // namespace services
