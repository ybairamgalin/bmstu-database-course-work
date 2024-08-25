#include "file_storage.hpp"

#include <chrono>

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fmt/format.h>
#include <boost/uuid/uuid_io.hpp>

namespace repository {

YandexS3FileStorage::YandexS3FileStorage(
    std::shared_ptr<Aws::S3::S3Client> s3_client)
    : s3_client_(std::move(s3_client)) {}

void YandexS3FileStorage::UploadFile(std::string&& file_content,
                                     boost::uuids::uuid uuid) {
  Aws::S3::Model::PutObjectRequest object_request;
  object_request.SetBucket("db-course-work");
  object_request.SetKey(boost::uuids::to_string(uuid));

  auto input_data = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
  *input_data << file_content;
  object_request.SetBody(input_data);
  auto outcome = s3_client_->PutObject(object_request);

  if (!outcome.IsSuccess()) {
    LOG_ERROR() << fmt::format("Could not upload file to s3: {}",
                               outcome.GetError().GetMessage());
    throw FileStorageException(fmt::format("Failed to upload file: {}",
                                           outcome.GetError().GetMessage()));
  }
}

std::optional<std::string> YandexS3FileStorage::GetFile(
    boost::uuids::uuid uuid) {
  return std::string();
}

}  // namespace repository
