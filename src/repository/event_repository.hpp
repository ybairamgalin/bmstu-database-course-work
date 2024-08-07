#pragma once

#include <vector>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Event {
  boost::uuids::uuid uuid;
  std::string name;
};

class EventRepository {
 public:
  virtual ~EventRepository() = default;
  virtual std::vector<Event> GetEventsByIds(
      const std::vector<boost::uuids::uuid>& event_id) = 0;
};

}  // namespace repository
