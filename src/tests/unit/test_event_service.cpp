#include <userver/utest/utest.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <gtest/gtest.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "base_service_test.hpp"

#include "services/auth_service.hpp"
#include "services/event/event.hpp"
#include "services/exception.hpp"
#include "services/request_management/request_management.hpp"

using namespace ::testing;

namespace {

class EventServiceTest : public BaseServiceTest {
 protected:
  void SetUp() override {
    BaseServiceTest::SetUp();
    service_ = std::make_unique<services::EventService>(repository_factory_);
  }

 protected:
  std::unique_ptr<services::EventService> service_{};
};

}  // namespace

TEST_F(EventServiceTest, GetAllEvents_Success) {
  std::vector<repository::Event> mock_repository_events;
  boost::uuids::uuid event_id_1 = boost::uuids::random_generator()();
  mock_repository_events.emplace_back(
      repository::Event{event_id_1, "event_1", "event_description_1"});

  boost::uuids::uuid event_id_2 = boost::uuids::random_generator()();
  mock_repository_events.emplace_back(
      repository::Event{event_id_2, "event_2", "event_description_2"});

  EXPECT_CALL(*events_repository_, GetAll())
      .WillOnce(testing::Return(mock_repository_events));

  auto events = service_->GetAllEvents();

  EXPECT_EQ(events.size(), 2);
  EXPECT_EQ(events.at(0).name, "event_1");
  EXPECT_EQ(events.at(0).description, "event_description_1");
  EXPECT_EQ(events.at(1).name, "event_2");
  EXPECT_EQ(events.at(1).description, "event_description_2");
}

TEST_F(EventServiceTest, AddEvent_Success) {
  services::Event event{"name", "description"};

  repository::Event added_event;

  EXPECT_CALL(*events_repository_, AddEvent(_))
      .WillOnce(SaveArg<0>(&added_event));

  auto uuid = service_->AddEvent(event);

  EXPECT_EQ(added_event.uuid, uuid);
  EXPECT_EQ(added_event.name, "name");
  EXPECT_EQ(added_event.description, "description");
}
