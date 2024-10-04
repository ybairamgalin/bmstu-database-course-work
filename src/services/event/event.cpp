#include "event.hpp"

#include <boost/uuid/uuid_generators.hpp>

namespace services {

EventService::EventService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : event_repository_(repository_factory->MakeEventsRepository()) {}

boost::uuids::uuid EventService::AddEvent(const Event& event) {
  auto uuid = boost::uuids::random_generator()();
  event_repository_->AddEvent(
      repository::Event{uuid, event.name, event.description});
  return uuid;
}

std::vector<Event> EventService::GetAllEvents() {
  auto events = event_repository_->GetAll();
  std::vector<Event> result;
  result.reserve(events.size());
  for (auto& event : events) {
    result.emplace_back(
        Event{std::move(event.name), std::move(event.description)});
  }
  return result;
}

}  // namespace services
