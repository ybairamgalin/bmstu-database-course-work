#include "test_event_repository.hpp"

#include <iostream>

#include <fmt/format.h>
#include <boost/uuid/uuid_generators.hpp>

#include <userver/utils/assert.hpp>

#include "repository/event_repository.hpp"
#include "repository/factory.hpp"
#include "repository/requests_repository.hpp"
#include "runner.hpp"

namespace {

class EventMother {
 public:
  repository::Event NewEvent() {
    return repository::Event{boost::uuids::random_generator()(), "event name",
                             "event description"};
  }
};

void TestAddEventSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  auto event = EventMother().NewEvent();

  auto events_repository = repository_factory->MakeEventsRepository();
  events_repository->AddEvent(event);

  auto all_events = events_repository->GetAll();
  LOG_ERROR() << all_events.size();
  UASSERT(all_events.size() == 1);
  UASSERT(all_events.front().uuid == event.uuid);
  UASSERT(all_events.front().name == event.name);
  UASSERT(all_events.front().description == event.description);
}

void TestGetAllEmpty(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  auto events_repository = repository_factory->MakeEventsRepository();
  auto all_events = events_repository->GetAll();

  UASSERT(all_events.size() == 0);
}

void TestGetAllSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  auto events_repository = repository_factory->MakeEventsRepository();
  events_repository->AddEvent(EventMother().NewEvent());
  events_repository->AddEvent(EventMother().NewEvent());

  auto all_events = events_repository->GetAll();

  UASSERT(all_events.size() == 2);
}

void TestGetEventsByIdsEmpty(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  auto events_repository = repository_factory->MakeEventsRepository();
  auto events =
      events_repository->GetEventsByIds({boost::uuids::random_generator()()});

  UASSERT(events.size() == 0);
}

void TestGetEventsByIdsSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  auto events_repository = repository_factory->MakeEventsRepository();
  auto event = EventMother().NewEvent();
  events_repository->AddEvent(event);

  auto events = events_repository->GetEventsByIds({event.uuid});

  UASSERT(events.size() == 1);
  UASSERT(events.front().uuid == event.uuid);
  UASSERT(events.front().name == event.name);
  UASSERT(events.front().description == event.description);
}

}  // namespace

void AddEventRepositoryTests() {
  TestRunner::AddSuite(
      "EventRepository",
      {DECL_TEST(TestAddEventSuccess), DECL_TEST(TestGetAllEmpty),
       DECL_TEST(TestGetAllSuccess), DECL_TEST(TestGetEventsByIdsEmpty),
       DECL_TEST(TestGetEventsByIdsSuccess)});
}
