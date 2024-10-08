#pragma once

#include "repository/factory.hpp"
#include "services/event_service.hpp"

namespace services {

class EventService : public IEventService {
 public:
  explicit EventService(const std::shared_ptr<repository::IRepositoryFactory>&
                            repository_factory);

  std::vector<EventFull> GetEvents(
      const std::vector<boost::uuids::uuid>& uuids) override;
  boost::uuids::uuid AddEvent(const services::Event& event) override;
  boost::uuids::uuid UpdateEvent(const services::Event& event,
                                 const boost::uuids::uuid& id) override;
  std::vector<Event> GetAllEvents() override;

 private:
  std::unique_ptr<repository::EventRepository> event_repository_;
};

}  // namespace services
