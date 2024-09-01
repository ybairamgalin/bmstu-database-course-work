#pragma once

#include <vector>
#include <string>

#include <boost/uuid/uuid.hpp>

namespace repository {

struct Event {
  boost::uuids::uuid uuid;
  std::string name;
  std::string description;
};

class EventRepository {
 public:
  virtual ~EventRepository() = default;
  virtual std::vector<Event> GetEventsByIds(
      const std::vector<boost::uuids::uuid>& event_id) = 0;
  virtual void AddEvent(const Event& event) = 0;
  virtual std::vector<Event> GetAll() = 0;
};

}  // namespace repository
