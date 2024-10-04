#include "request_management_service.hpp"

#include "services/auth_service.hpp"

namespace services {

RequestToCreateOrUpdateBuilder::RequestToCreateOrUpdateBuilder() : request_{} {
  request_.status = "new";
}

RequestToCreateOrUpdateBuilder& RequestToCreateOrUpdateBuilder::WithEvent(
    const boost::uuids::uuid& event_id) {
  request_.event_id = event_id;
  return *this;
}

RequestToCreateOrUpdateBuilder& RequestToCreateOrUpdateBuilder::WithDescription(
    const std::string& description) {
  request_.description = description;
  return *this;
}

RequestToCreateOrUpdateBuilder& RequestToCreateOrUpdateBuilder::WithStatus(
    const std::string& status) {
  request_.status = status;
  return *this;
}


RequestToCreateOrUpdateBuilder& RequestToCreateOrUpdateBuilder::WithAuthor(
    const AuthData& auth_data) {
  request_.author = auth_data;
  return *this;
}

RequestToCreateOrUpdate RequestToCreateOrUpdateBuilder::Build() {
  return request_;
}

}  // namespace services
