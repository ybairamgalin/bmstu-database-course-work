#pragma once

#include "repository/factory.hpp"
#include "services/event_service.hpp"

namespace services {

class EventService : public IEventService {
 public:
  explicit EventService(const std::shared_ptr<repository::IRepositoryFactory>&
                            repository_factory);

  boost::uuids::uuid AddEvent(const services::Event& event) override;
  std::vector<Event> GetAllEvents() override;
  void UpdateEvent(const boost::uuids::uuid& id, const services::Event& event,
                   const services::AuthData& auth) override;
  std::vector<Event> SearchEvents(const std::string& substring) override;
  void DeleteEvent(const boost::uuids::uuid& id,
                   const services::AuthData& auth) override;
  std::optional<EventFull> GetEventById(const boost::uuids::uuid& id) override;

 private:
  std::unique_ptr<repository::EventRepository> event_repository_;
};

}  // namespace services
