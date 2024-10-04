#include <userver/utest/utest.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include <gtest/gtest.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "base_service_test.hpp"

#include <userver/crypto/hash.hpp>

#include "services/auth_service.hpp"
#include "services/event/event.hpp"
#include "services/exception.hpp"
#include "services/file_service/file_service.hpp"
#include "services/request_management/request_management.hpp"

using namespace ::testing;

namespace {

class FileServiceTest : public BaseServiceTest {
 protected:
  void SetUp() override {
    BaseServiceTest::SetUp();
    service_ = std::make_unique<services::FileService>(repository_factory_);
  }

 protected:
  std::unique_ptr<services::FileService> service_{};
};

}  // namespace

TEST_F(FileServiceTest, UploadFile_Success) {
  services::File file{"file content"};
  auto hash = userver::crypto::hash::Sha256(file.content);
  boost::uuids::uuid expected_uuid = boost::uuids::random_generator()();

  EXPECT_CALL(*file_meta_repository_, GetFileMetaByHash(hash))
      .WillOnce(Return(std::nullopt));
  EXPECT_CALL(*file_storage_repository_, UploadFile(_, _)).WillOnce(Return());
  EXPECT_CALL(*file_meta_repository_, UpsertFileMeta(_)).WillOnce(Return());

  auto result_uuid = service_->UploadFile(std::move(file));
  EXPECT_NE(result_uuid,
            boost::uuids::uuid{});  // Проверяем, что UUID не пустой
}

TEST_F(FileServiceTest, UploadFile_FileStorageException) {
  services::File file{"file content"};
  auto hash = userver::crypto::hash::Sha256(file.content);

  EXPECT_CALL(*file_meta_repository_, GetFileMetaByHash(hash))
      .WillOnce(Return(std::nullopt));
  EXPECT_CALL(*file_storage_repository_, UploadFile(_, _))
      .WillOnce(Throw(repository::FileStorageException("Upload failed")));

  EXPECT_THROW(service_->UploadFile(std::move(file)),
               services::ServiceLevelException);
}

// Тесты для метода GetFile

TEST_F(FileServiceTest, GetFile_Success) {
  boost::uuids::uuid file_uuid = boost::uuids::random_generator()();
  std::string file_content = "file content";

  EXPECT_CALL(*file_storage_repository_, GetFile(file_uuid))
      .WillOnce(Return(std::make_optional(file_content)));
  EXPECT_CALL(*file_meta_repository_, GetFileMetaByFileUuid(file_uuid))
      .WillOnce(Return(repository::FileMeta{file_uuid, "some_hash"}));

  auto result = service_->GetFile(file_uuid);
  ASSERT_TRUE(result.has_value());
}

TEST_F(FileServiceTest, GetFile_FileNotFound) {
  boost::uuids::uuid file_uuid = boost::uuids::random_generator()();

  EXPECT_CALL(*file_storage_repository_, GetFile(file_uuid))
      .WillOnce(Return(std::nullopt));

  auto result = service_->GetFile(file_uuid);
  EXPECT_FALSE(result.has_value());
}
