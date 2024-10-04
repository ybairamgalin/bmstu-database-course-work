#include <userver/utils/datetime/timepoint_tz.hpp>

#include <gtest/gtest.h>

#include "base_service_test.hpp"
#include "request_object_mother.hpp"
#include "tests/object_mother.hpp"

#include "services/auth_service.hpp"
#include "services/exception.hpp"
#include "services/idm/simple_idm_service.hpp"

using namespace ::testing;

namespace {

class IdmRequestMother {
 public:
  services::IdmRequest NewIdmRequest(services::AuthRole author_role) {
    return services::IdmRequest{
        "user_login",
        services::UserRole::kAdmin,
        services::AuthData{1, "my_login", "name", author_role, {}},
        {},
    };
  }
};

class IdmServiceTest : public BaseServiceTest {
 protected:
  void SetUp() override {
    BaseServiceTest::SetUp();
    service_ =
        std::make_unique<services::SimpleIdmService>(repository_factory_);
  }

 protected:
  std::unique_ptr<services::IIdmService> service_{};
};

}  // namespace

TEST_F(IdmServiceTest, TestHandleIdmRequestNotAllowed) {
  EXPECT_THROW(service_->HandleIdmRequest(
                   IdmRequestMother().NewIdmRequest(services::AuthRole::kUser)),
               services::ServiceLevelException);
}

TEST_F(IdmServiceTest, TestHandleIdmRequestSuccess) {
  auto idm_request =
      IdmRequestMother().NewIdmRequest(services::AuthRole::kAdmin);
  auto db_user = repository::AuthDataMother().NewUser();

  EXPECT_CALL(*user_data_repository_, GetUserDataByLogin(idm_request.login))
      .WillOnce(Return(db_user));
  EXPECT_CALL(*user_data_repository_, SerUserRole(idm_request.login, "admin"));

  service_->HandleIdmRequest(std::move(idm_request));
}
