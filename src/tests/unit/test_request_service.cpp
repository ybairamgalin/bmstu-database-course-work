#include <userver/utest/utest.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <gtest/gtest.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "base_service_test.hpp"
#include "request_object_mother.hpp"
#include "tests/object_mother.hpp"

#include "services/auth_service.hpp"
#include "services/exception.hpp"
#include "services/request_management/request_management.hpp"
#include "services/request_management_service.hpp"

namespace {

class RequestsServiceTest : public BaseServiceTest {
 protected:
  void SetUp() override {
    BaseServiceTest::SetUp();
    service_ = std::make_unique<services::RequestManagementService>(
        repository_factory_);
  }

 protected:
  std::unique_ptr<services::RequestManagementService> service_{};
};

}  // namespace

TEST_F(RequestsServiceTest, GetRequestById_Success) {
  services::AuthData auth{1, "login", "name", services::AuthRole::kUser, {}};

  auto db_request = repository::RequestFullObjectMother().GetNewRequest();
  auto request_id = db_request.request_id;

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  auto db_auth_data = repository::AuthDataMother().NewUser();

  EXPECT_CALL(*user_data_repository_, GetUserDataByIds(testing::ElementsAre(1)))
      .WillOnce(
          testing::Return(std::vector<repository::AuthData>{db_auth_data}));

  auto result = service_->GetRequestById(request_id, auth);

  EXPECT_EQ(result.author.id, db_request.author_id);
  EXPECT_EQ(result.author.name, db_auth_data.name);
  EXPECT_EQ(result.author.login, db_auth_data.login);
  EXPECT_EQ(result.status, db_request.status);
  EXPECT_EQ(result.description, db_request.description);
}

TEST_F(RequestsServiceTest, GetRequestById_AccessDenied) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  services::AuthData auth{2, "login", "name", services::AuthRole::kUser, {}};

  auto db_request = repository::RequestFullObjectMother().GetNewRequest();

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  EXPECT_THROW(service_->GetRequestById(request_id, auth),
               services::ServiceLevelException);
}

TEST_F(RequestsServiceTest, AddRequest_Success) {
  auto request = services::RequestToCreateOrUpdateBuilder()
                     .WithEvent(boost::uuids::random_generator()())
                     .WithAuthor(services::AuthData{
                         1, "login", "name", services::AuthRole::kUser, {}})
                     .WithDescription("Test request")
                     .Build();

  repository::Event db_event{};
  db_event.uuid = request.event_id;

  EXPECT_CALL(*events_repository_,
              GetEventsByIds(testing::ElementsAre(request.event_id)))
      .WillOnce(testing::Return(std::vector<repository::Event>{db_event}));

  EXPECT_CALL(*requests_repository_, Insert(testing::_))
      .WillOnce(testing::Return());

  auto result = service_->AddRequest(request);

  EXPECT_FALSE(result.is_nil());
}

TEST_F(RequestsServiceTest, UpdateRequest_Success) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  auto request = services::RequestToCreateOrUpdateBuilder()
                     .WithEvent(boost::uuids::random_generator()())
                     .WithAuthor(services::AuthData{
                         1, "login", "name", services::AuthRole::kAdmin, {}})
                     .WithDescription("updated request")
                     .WithStatus("updated")
                     .Build();

  auto db_request = repository::RequestFullObjectMother().GetNewRequest();

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  EXPECT_CALL(*requests_repository_, Update(testing::_))
      .WillOnce(testing::Return());

  service_->UpdateRequest(request_id, request);
}

TEST_F(RequestsServiceTest, AddComment_Success) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  std::string content = "Test comment";
  services::AuthData auth{1, "login", "name", services::AuthRole::kUser, {}};

  auto db_request = repository::RequestFullObjectMother().GetNewRequest();

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  EXPECT_CALL(*requests_repository_,
              AddComment(request_id, content, auth.user_id))
      .WillOnce(testing::Return());

  service_->AddComment(request_id, content, auth);
}

TEST_F(RequestsServiceTest, GetAll_Success) {
  services::AuthData auth{
      1, "admin", "admin_name", services::AuthRole::kAdmin, {}};

  repository::RequestShort db_request{};
  db_request.request_id = boost::uuids::random_generator()();
  db_request.author_id = 1;
  db_request.created_at = userver::storages::postgres::Now();

  EXPECT_CALL(*requests_repository_, GetAll())
      .WillOnce(
          testing::Return(std::vector<repository::RequestShort>{db_request}));

  auto db_auth_data = repository::AuthDataMother().NewUser();

  EXPECT_CALL(*user_data_repository_, GetUserDataByIds(testing::ElementsAre(1)))
      .WillOnce(
          testing::Return(std::vector<repository::AuthData>{db_auth_data}));

  auto result = service_->GetAll(auth);

  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].author.id, db_auth_data.user_id);
  EXPECT_EQ(result[0].author.name, db_auth_data.name);
  EXPECT_EQ(result[0].author.login, db_auth_data.login);
}

TEST_F(RequestsServiceTest, TestFilterRequestsByDate) {
  EXPECT_TRUE(false);
}

TEST_F(RequestsServiceTest, TestFilterRequestsById) {
  EXPECT_TRUE(true);
}

TEST_F(RequestsServiceTest, TestFilterRequestsByAuthor) {
  EXPECT_TRUE(true);
}

