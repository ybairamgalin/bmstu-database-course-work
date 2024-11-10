#pragma once

#include <userver/utest/utest.hpp>

#include <gtest/gtest.h>

#include "services/auth_service.hpp"
#include "mock.hpp"
#include "services/exception.hpp"
#include "services/request_management/request_management.hpp"


class BaseServiceTest : public ::testing::Test {
 protected:
  void SetUp() override;

 protected:
  std::shared_ptr<repository::MockRepositoryFactory> repository_factory_{};

  repository::MockRequestsRepository* requests_repository_{};
  repository::MockEventRepository* events_repository_{};
  repository::MockUserDataRepository* user_data_repository_{};
  repository::MockFileStorageRepository* file_storage_repository_{};
  repository::MockFileMetaRepository* file_meta_repository_{};
};
