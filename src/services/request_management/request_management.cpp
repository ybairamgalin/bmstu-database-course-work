#include "request_management.hpp"

#include "services/exception.hpp"

namespace services {

RequestManagementService::RequestManagementService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : request_repository_(repository_factory->MakeRequestsRepository()) {}

void RequestManagementService::AddRequest(const Request& request) {
  repository::Request db_request{0, request.event_id, request.author_id,
                                 request.description, request.attachment_ids};
  request_repository_->Insert(db_request);
}

void RequestManagementService::UpdateRequest(const Request& request) {
  if (!request.request_id.has_value()) {
    throw services::ServiceLevelException(
        "Id should be set for update request");
  }
  repository::Request db_request{
      request.request_id.value(), request.event_id,       request.author_id,
      request.description,        request.attachment_ids,
  };
  request_repository_->Update(db_request);
}

}  // namespace services
