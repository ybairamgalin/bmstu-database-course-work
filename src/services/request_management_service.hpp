
#pragma once

#include <optional>
#include <string>

namespace services {

struct Request {
  std::optional<int64_t> request_id;
  std::string description;
};

class IRequestManagementService {
 public:
  virtual void AddRequest(const Request& request) = 0;
  virtual void UpdateRequest(const Request& request) = 0;
};

}  // namespace services
