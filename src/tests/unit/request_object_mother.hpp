#pragma once

#include <repository/requests_repository.hpp>
#include <services/request_management_service.hpp>

namespace repository {

class RequestFullObjectMother {
 public:
  repository::RequestFull GetNewRequest();
};

}  // namespace repository
