#pragma once

#include <optional>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

namespace services {

struct Event {
  boost::uuids::uuid id;
  std::string name;
  std::string description;
};

struct EventFull {
  boost::uuids::uuid id;
  std::string name;
  std::string description;
};

class IEventService {
 public:
  virtual ~IEventService() = default;
  virtual boost::uuids::uuid AddEvent(const Event& event) = 0;
  virtual void UpdateEvent(const boost::uuids::uuid& id, const Event& event,
                           const AuthData& auth) = 0;
  virtual std::vector<Event> SearchEvents(const std::string& substring) = 0;
  virtual std::vector<Event> GetAllEvents() = 0;
  virtual void DeleteEvent(const boost::uuids::uuid& id,
                           const AuthData& auth) = 0;
  virtual std::optional<EventFull> GetEventById(
      const boost::uuids::uuid& id) = 0;
};

}  // namespace services
