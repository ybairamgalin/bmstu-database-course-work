#include "event.hpp"

#include <boost/uuid/uuid_generators.hpp>

namespace services {

EventService::EventService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : event_repository_(repository_factory->MakeEventsRepository()) {}

std::vector<EventFull> EventService::GetEvents(
    const std::vector<boost::uuids::uuid>& uuids) {
  return {};
}

boost::uuids::uuid EventService::AddEvent(const Event& event) {
  auto uuid = boost::uuids::random_generator()();
  event_repository_->AddEvent(
      repository::Event{uuid, event.name, event.description});
  return uuid;
}

boost::uuids::uuid EventService::UpdateEvent(const Event& event,
                                             const boost::uuids::uuid& id) {
  return {};
}

}  // namespace services
