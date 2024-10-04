#pragma once

#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

namespace services {

struct Event {
  std::string name;
  std::string description;
};

struct EventFull {
  std::string name;
  std::string description;
};

class IEventService {
 public:
  virtual ~IEventService() = default;
  virtual boost::uuids::uuid AddEvent(const Event& event) = 0;
  virtual std::vector<Event> GetAllEvents() = 0;
};

}  // namespace services
