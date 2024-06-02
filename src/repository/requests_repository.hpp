#pragma once

#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Request {
  int64_t request_id;
  int64_t event_id;
  int64_t author_id;
  std::string description;
  std::vector<boost::uuids::uuid> attachment_ids;
};

class RequestsRepository {
 public:
  virtual ~RequestsRepository() = default;
  virtual void Insert(const Request& request) = 0;
  virtual void Update(const Request& request) = 0;
};

}  // namespace repository
