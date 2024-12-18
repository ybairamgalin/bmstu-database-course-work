#include "request_management.hpp"

#include <string_view>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <userver/utils/datetime/timepoint_tz.hpp>

#include "services/exception.hpp"
#include "utils/map.hpp"

namespace {

std::vector<repository::Attachment> MapAttachments(
    const std::vector<services::Attachment>& attachments) {
  std::vector<repository::Attachment> result;
  result.reserve(attachments.size());
  for (const auto& attachment : attachments) {
    result.emplace_back(
        repository::Attachment{attachment.id, attachment.filename});
  }
  return result;
}

std::vector<services::AttachmentForDownload> MapAttachments(
    const std::vector<repository::Attachment>& attachments) {
  static constexpr std::string_view kS3Host = "storage.yandexcloud.net";
  static constexpr std::string_view kS3Bucket = "db-course-work";

  std::vector<services::AttachmentForDownload> result;
  result.reserve(attachments.size());
  for (const auto& attachment : attachments) {
    result.emplace_back(services::AttachmentForDownload{
        fmt::format("https://{}/{}/{}", kS3Host, kS3Bucket,
                    boost::uuids::to_string(attachment.id)),
        attachment.filename});
  }
  return result;
}

}  // namespace

namespace services {

RequestManagementService::RequestManagementService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : request_repository_(repository_factory->MakeRequestsRepository()),
      event_repository_(repository_factory->MakeEventsRepository()),
      user_data_repository_(repository_factory->MakeUserDataDbRepository()) {}

Request RequestManagementService::GetRequestById(
    const boost::uuids::uuid& request_id, const AuthData& auth) {
  auto request_opt = request_repository_->GetRequestById(request_id);
  if (!request_opt) {
    throw ServiceLevelException("Request not found");
  }
  auto& request = request_opt.value();

  if (auth.role == AuthRole::kUser && auth.user_id != request.author_id) {
    throw ServiceLevelException("You cannot view this request",
                                ErrorType::kPermissionDenied);
  }

  std::vector<int64_t> user_ids_to_request;
  user_ids_to_request.emplace_back(request.author_id);
  for (const auto& comment : request.comments) {
    user_ids_to_request.emplace_back(comment.author_id);
  }

  auto user_id_to_user = utils::AsMap(
      user_data_repository_->GetUserDataByIds(user_ids_to_request),
      [](const repository::AuthData& elem) -> int64_t { return elem.user_id; });

  if (!user_id_to_user.count(request.author_id)) {
    throw std::runtime_error("Author is references but not found");
  }
  Request result{};

  result.author = MapUser(user_id_to_user.at(request.author_id));
  result.status = request.status;
  result.event_id = request.event_id;
  result.description = request.description;
  result.attachments = MapAttachments(request.attachments);
  result.created_at =
      userver::utils::datetime::TimePointTz{request.created_at.GetUnderlying()};
  result.updated_at =
      userver::utils::datetime::TimePointTz{request.updated_at.GetUnderlying()};

  for (const auto& comment : request.comments) {
    if (!user_id_to_user.count(comment.author_id)) {
      throw std::runtime_error("Comment author is references but not found");
    }
    result.comments.emplace_back(services::RequestComment{
        comment.content, MapUser(user_id_to_user.at(comment.author_id)),
        userver::utils::datetime::TimePointTz{
            comment.created_at.GetUnderlying()}});
  }

  return result;
}

boost::uuids::uuid RequestManagementService::AddRequest(
    const RequestToCreateOrUpdate& request) {
  auto events = event_repository_->GetEventsByIds({request.event_id});
  if (events.empty()) {
    throw ServiceLevelException("Event does not exist");
  }

  auto boost_uuid = boost::uuids::random_generator()();

  repository::Request db_request{boost_uuid,
                                 request.event_id,
                                 request.author.user_id,
                                 request.description,
                                 MapAttachments(request.attachments),
                                 "new"};
  request_repository_->Insert(db_request);
  return boost_uuid;
}

void RequestManagementService::UpdateRequest(
    const boost::uuids::uuid& request_id,
    const RequestToCreateOrUpdate& orig_request) {
  auto request = orig_request;
  auto db_request_opt = request_repository_->GetRequestById(request_id);
  if (!db_request_opt.has_value()) {
    throw ServiceLevelException("Request not found");
  }
  if (db_request_opt->author_id != request.author.user_id) {
    throw ServiceLevelException("Cannot edit other users' requests",
                                ErrorType::kInvalidInput);
  }
  if (request.status.empty()) {
    request.status = db_request_opt.value().status;
  }
  if (db_request_opt.value().status != request.status &&
      request.author.role == AuthRole::kUser) {
    throw ServiceLevelException("Cannot edit request status",
                                ErrorType::kPermissionDenied);
  }
  repository::Request db_request{
      request_id,
      request.event_id,
      request.author.user_id,
      request.description,
      MapAttachments(request.attachments),
      request.status,
  };
  request_repository_->Update(db_request);
}

void RequestManagementService::AddComment(const boost::uuids::uuid& request_id,
                                          const std::string& content,
                                          const AuthData& auth) {
  auto request_opt = request_repository_->GetRequestById(request_id);
  if (!request_opt.has_value()) {
    throw ServiceLevelException("Request does not exist");
  }
  if (auth.role == AuthRole::kUser &&
      auth.user_id != request_opt.value().author_id) {
    throw ServiceLevelException("You cannot add comment to this request");
  }

  request_repository_->AddComment(request_id, content, auth.user_id);
}

std::vector<RequestShort> RequestManagementService::GetAll(
    const AuthData& auth) {
  if (auth.role == AuthRole::kUser) {
    throw ServiceLevelException("Forbidden");
  }

  auto db_requests = request_repository_->GetAll();
  return MapRequests(db_requests);
}

std::vector<RequestShort> RequestManagementService::GetFiltered(
    const RequestFilters& filters, const AuthData& auth) {
  repository::RequestFilters repository_filters;
  if (auth.role == AuthRole::kUser) {
    if (filters.author_login.has_value() &&
        filters.author_login.value() != auth.login) {
      return {};
    }
    repository_filters.author_id.emplace(auth.user_id);
  } else {
    if (filters.author_login.has_value()) {
      auto user_data = user_data_repository_->GetUserDataByLogin(
          filters.author_login.value());
      if (!user_data.has_value()) {
        throw ServiceLevelException("User not found");
      }
      repository_filters.author_id = user_data.value().user_id;
    }
  }
  repository_filters.request_id = filters.request_id;
  repository_filters.event_id = filters.event_id;
  repository_filters.created_after =
      filters.created_after.has_value()
          ? userver::storages::postgres::TimePointTz{filters.created_after
                                                         .value()}
          : std::optional<userver::storages::postgres::TimePointTz>();
  return MapRequests(request_repository_->GetFiltered(repository_filters));
}

std::vector<services::RequestShort> RequestManagementService::MapRequests(
    const std::vector<repository::RequestShort>& db_requests) {
  std::vector<int64_t> user_ids;
  user_ids.reserve(db_requests.size());
  for (const auto& db_request : db_requests) {
    user_ids.emplace_back(db_request.author_id);
  }
  auto user_id_to_user = utils::AsMap(
      user_data_repository_->GetUserDataByIds(user_ids),
      [](const repository::AuthData& elem) -> int64_t { return elem.user_id; });

  std::vector<RequestShort> result;
  result.reserve(db_requests.size());
  for (const auto& db_request : db_requests) {
    result.emplace_back(RequestShort{
        db_request.request_id,
        MapUser(user_id_to_user.at(db_request.author_id)),
        userver::utils::datetime::TimePointTz{db_request.created_at},
    });
  }

  return result;
}

}  // namespace services
