#include "base_service_test.hpp"

#include <memory>

void BaseServiceTest::SetUp() {
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

  auto tmp_file_meta_repository =
      std::make_unique<repository::MockFileMetaRepository>();
  file_meta_repository_ = tmp_file_meta_repository.get();

  auto tmp_file_storage_repository =
      std::make_unique<repository::MockFileStorageRepository>();
  file_storage_repository_ = tmp_file_storage_repository.get();

  EXPECT_CALL(*repository_factory_, MakeRequestsRepository())
      .Times(::testing::AnyNumber())
      .WillOnce(
          testing::Return(testing::ByMove(std::move(tmp_requests_repository))));

  EXPECT_CALL(*repository_factory_, MakeEventsRepository())
      .Times(::testing::AnyNumber())
      .WillOnce(
          testing::Return(testing::ByMove(std::move(tmp_events_repository))));

  EXPECT_CALL(*repository_factory_, MakeUserDataDbRepository())
      .Times(::testing::AnyNumber())
      .WillOnce(testing::Return(
          testing::ByMove(std::move(tmp_user_data_repository))));

  EXPECT_CALL(*repository_factory_, MakeFileMetaRepository())
      .Times(::testing::AnyNumber())
      .WillOnce(testing::Return(
          testing::ByMove(std::move(tmp_file_meta_repository))));

  EXPECT_CALL(*repository_factory_, MakeFileStorageRepository())
      .Times(::testing::AnyNumber())
      .WillOnce(testing::Return(
          testing::ByMove(std::move(tmp_file_storage_repository))));
}
