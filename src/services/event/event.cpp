#include "event.hpp"

#include <services/exception.hpp>

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

void EventService::UpdateEvent(const boost::uuids::uuid& id, const Event& event,
                               const AuthData& auth) {
  if (auth.role == AuthRole::kUser) {
    throw ServiceLevelException("You cannot edit events",
                                ErrorType::kPermissionDenied);
  }

  event_repository_->AddEvent(
      repository::Event{id, event.name, event.description});
}

std::vector<Event> EventService::SearchEvents(const std::string& substring) {
  const auto events = GetAllEvents();
  if (substring.empty()) {
    return events;
  }

  std::vector<Event> result;
  for (const auto& event : events) {
    if (event.name.find(substring) != std::string::npos) {
      result.emplace_back(event);
    }
  }
  return result;
}

}  // namespace services
