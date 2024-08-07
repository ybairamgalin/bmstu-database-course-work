#include "request_management.hpp"

#include <userver/utils/datetime/timepoint_tz.hpp>

#include <boost/uuid/uuid_generators.hpp>

#include "services/exception.hpp"

namespace services {

RequestManagementService::RequestManagementService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : request_repository_(repository_factory->MakeRequestsRepository()),
      event_repository_(repository_factory->MakeEventsRepository()),
      user_data_repository_(repository_factory->MakeUserDataDbRepository()) {}

Request RequestManagementService::GetRequestById(
    const boost::uuids::uuid& request_id) {
  auto requests = request_repository_->GetRequestsByIds({request_id});
  if (requests.empty()) {
    throw ServiceLevelException("Request not found");
  }
  auto& request = requests.front();
  auto user = user_data_repository_->GetUserDataById(request.author_id);
  if (!user.has_value()) {
    throw std::runtime_error("User not found but is referenced in other table");
  }

  Request result{};
  result.author.name = user.value().name;
  result.author.login = user.value().login;
  result.author.id = user.value().user_id;
  result.author.phone = user.value().phone;

  result.event_id = request.event_id;
  result.description = request.description;
  result.created_at =
      userver::utils::datetime::TimePointTz{request.created_at.GetUnderlying()};
  result.updated_at =
      userver::utils::datetime::TimePointTz{request.updated_at.GetUnderlying()};

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

}  // namespace services
