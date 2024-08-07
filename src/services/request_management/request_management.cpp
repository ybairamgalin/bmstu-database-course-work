#include "request_management.hpp"

#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid_generators.hpp>

#include "services/exception.hpp"
#include "utils/map.hpp"

namespace {

services::UserInfo MapUser(const repository::AuthData& user) {
  return services::UserInfo{user.user_id, user.name, user.login, user.phone};
}

}  // namespace

namespace services {

RequestManagementService::RequestManagementService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : request_repository_(repository_factory->MakeRequestsRepository()),
      event_repository_(repository_factory->MakeEventsRepository()),
      user_data_repository_(repository_factory->MakeUserDataDbRepository()) {}

Request RequestManagementService::GetRequestById(
    const boost::uuids::uuid& request_id) {
  auto request_opt = request_repository_->GetRequestById(request_id);
  if (!request_opt) {
    throw ServiceLevelException("Request not found");
  }
  auto& request = request_opt.value();

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
  result.event_id = request.event_id;
  result.description = request.description;
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
  repository::Request db_request{boost_uuid, request.event_id,
                                 request.author_id, request.description,
                                 request.attachment_ids};
  request_repository_->Insert(db_request);
  return boost_uuid;
}

void RequestManagementService::UpdateRequest(
    const boost::uuids::uuid& request_id,
    const RequestToCreateOrUpdate& request) {
  repository::Request db_request{
      request_id,          request.event_id,       request.author_id,
      request.description, request.attachment_ids,
  };
  request_repository_->Update(db_request);
}

void RequestManagementService::AddComment(const boost::uuids::uuid& request_id,
                                          const std::string& content,
                                          int64_t author_id) {
  auto request_opt = request_repository_->GetRequestById(request_id);
  if (!request_opt.has_value()) {
    throw ServiceLevelException("Request does not exist");
  }
  request_repository_->AddComment(request_id, content, author_id);
}

}  // namespace services
