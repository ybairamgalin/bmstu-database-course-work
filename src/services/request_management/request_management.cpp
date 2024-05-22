#include "request_management.hpp"

namespace services {

RequestManagementService::RequestManagementService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {

}

void RequestManagementService::AddRequest(const Request& request) {}

void RequestManagementService::UpdateRequest(const Request& request) {}
}
