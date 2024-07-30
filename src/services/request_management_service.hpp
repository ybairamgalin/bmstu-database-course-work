
#pragma once

#include <optional>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace services {

struct Request {
  std::optional<int64_t> request_id;
  int64_t author_id;
  int64_t event_id;
  std::string description;
  std::vector<boost::uuids::uuid> attachment_ids;
};

class IRequestManagementService {
 public:
  virtual ~IRequestManagementService() = default;
  virtual void AddRequest(const Request& request) = 0;
  virtual void UpdateRequest(const Request& request) = 0;
};

}  // namespace services
