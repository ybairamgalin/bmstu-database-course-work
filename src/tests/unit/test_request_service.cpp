#include <userver/utest/utest.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <gtest/gtest.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "mock.hpp"

#include "services/auth_service.hpp"
#include "services/exception.hpp"
#include "services/request_management/request_management.hpp"

namespace {

class RequestsServiceTest : public testing::Test {
 protected:
  void SetUp() override {
    repository_factory_ = std::make_shared<repository::MockRepositoryFactory>();

    auto tmp_requests_repository =
        std::make_unique<repository::MockRequestsRepository>();
    requests_repository_ = tmp_requests_repository.get();

    auto tmp_events_repository =
        std::make_unique<repository::MockEventRepository>();
    events_repository_ = tmp_events_repository.get();

    auto tmp_user_data_repository =
        std::make_unique<repository::MockUserDataRepository>();
    user_data_repository_ = tmp_user_data_repository.get();

    EXPECT_CALL(*repository_factory_, MakeRequestsRepository())
        .WillOnce(testing::Return(
            testing::ByMove(std::move(tmp_requests_repository))));

    EXPECT_CALL(*repository_factory_, MakeEventsRepository())
        .WillOnce(
            testing::Return(testing::ByMove(std::move(tmp_events_repository))));

    EXPECT_CALL(*repository_factory_, MakeUserDataDbRepository())
        .WillOnce(testing::Return(
            testing::ByMove(std::move(tmp_user_data_repository))));

    service_ = std::make_unique<services::RequestManagementService>(
        repository_factory_);
  }

 protected:
  std::shared_ptr<repository::MockRepositoryFactory> repository_factory_{};

  repository::MockRequestsRepository* requests_repository_{};
  repository::MockEventRepository* events_repository_{};
  repository::MockUserDataRepository* user_data_repository_{};

  std::unique_ptr<services::RequestManagementService> service_{};
};

}  // namespace

TEST_F(RequestsServiceTest, GetRequestById_Success) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  services::AuthData auth{1, "login", "name", services::AuthRole::kUser, {}};

  const auto now = userver::storages::postgres::Now();

  repository::RequestFull db_request{};
  db_request.request_id = request_id;
  db_request.author_id = 1;
  db_request.status = "new";
  db_request.event_id = boost::uuids::random_generator()();
  db_request.description = "Test request";
  db_request.created_at = now;
  db_request.updated_at = now;

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  repository::AuthData db_auth_data{};
  db_auth_data.user_id = 1;
  db_auth_data.name = "name";
  db_auth_data.login = "test_user";

  EXPECT_CALL(*user_data_repository_, GetUserDataByIds(testing::ElementsAre(1)))
      .WillOnce(
          testing::Return(std::vector<repository::AuthData>{db_auth_data}));

  auto result = service_->GetRequestById(request_id, auth);

  EXPECT_EQ(result.author.id, 1);
  EXPECT_EQ(result.author.name, "name");
  EXPECT_EQ(result.author.login, "test_user");
  EXPECT_EQ(result.status, "new");
  EXPECT_EQ(result.description, "Test request");
}

TEST_F(RequestsServiceTest, GetRequestById_AccessDenied) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  services::AuthData auth{2, "login", "name", services::AuthRole::kUser, {}};

  const auto now = userver::storages::postgres::Now();

  repository::RequestFull db_request{};
  db_request.request_id = request_id;
  db_request.author_id = 1;
  db_request.status = "new";
  db_request.event_id = boost::uuids::random_generator()();
  db_request.description = "Test request";
  db_request.created_at = now;
  db_request.updated_at = now;

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  try {
    service_->GetRequestById(request_id, auth);
    FAIL() << "Expected ServiceLevelException";
  } catch (const services::ServiceLevelException& ex) {
    EXPECT_EQ(ex.GetErrorType(), services::ErrorType::kPermissionDenied);
  } catch(...) {
    FAIL() << "Expected ServiceLevelException";
  }
}

TEST_F(RequestsServiceTest, AddRequest_Success) {
  services::RequestToCreateOrUpdate request{};
  request.event_id = boost::uuids::random_generator()();
  request.author = {1, "login", "name", services::AuthRole::kUser, {}};
  request.description = "Test request";

  repository::Event db_event{};
  db_event.uuid = request.event_id;

  EXPECT_CALL(*events_repository_, GetEventsByIds(testing::ElementsAre(request.event_id)))
      .WillOnce(testing::Return(std::vector<repository::Event>{db_event}));

  EXPECT_CALL(*requests_repository_, Insert(testing::_))
      .WillOnce(testing::Return());

  auto result = service_->AddRequest(request);

  EXPECT_FALSE(result.is_nil());
}

TEST_F(RequestsServiceTest, UpdateRequest_Success) {
  boost::uuids::uuid request_id = boost::uuids::random_generator()();
  services::RequestToCreateOrUpdate request{};
  request.event_id = boost::uuids::random_generator()();
  request.author = {1, "login", "name", services::AuthRole::kAdmin, {}};
  request.description = "Updated request";
  request.status = "updated";

  repository::RequestFull db_request{};
  db_request.request_id = request_id;
  db_request.author_id = 1;
  db_request.status = "new";
  db_request.event_id = request.event_id;
  db_request.description = "Test request";

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

  repository::RequestFull db_request{};
  db_request.request_id = request_id;
  db_request.author_id = 1;

  EXPECT_CALL(*requests_repository_, GetRequestById(request_id))
      .WillOnce(testing::Return(db_request));

  EXPECT_CALL(*requests_repository_, AddComment(request_id, content, auth.user_id))
      .WillOnce(testing::Return());

  service_->AddComment(request_id, content, auth);
}

TEST_F(RequestsServiceTest, GetAll_Success) {
  services::AuthData auth{1, "admin", "admin_name", services::AuthRole::kAdmin, {}};

  repository::RequestShort db_request{};
  db_request.request_id = boost::uuids::random_generator()();
  db_request.author_id = 1;
  db_request.created_at = userver::storages::postgres::Now();

  EXPECT_CALL(*requests_repository_, GetAll())
      .WillOnce(testing::Return(std::vector<repository::RequestShort>{db_request}));

  repository::AuthData db_auth_data{};
  db_auth_data.user_id = 1;
  db_auth_data.name = "name";
  db_auth_data.login = "test_user";

  EXPECT_CALL(*user_data_repository_, GetUserDataByIds(testing::ElementsAre(1)))
      .WillOnce(
          testing::Return(std::vector<repository::AuthData>{db_auth_data}));

  auto result = service_->GetAll(auth);

  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].author.id, 1);
  EXPECT_EQ(result[0].author.name, "name");
  EXPECT_EQ(result[0].author.login, "test_user");
}
