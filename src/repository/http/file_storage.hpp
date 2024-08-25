#pragma once

#include <aws/s3/S3Client.h>
#include <userver/clients/http/client.hpp>

#include "../file_storage_repository.hpp"

namespace repository {

class YandexS3FileStorage : public FileStorageRepository {
 public:
  explicit YandexS3FileStorage(userver::clients::http::Client& http_client);

  void UploadFile(std::string&& file_content, boost::uuids::uuid uuid) override;
  std::optional<std::string> GetFile(boost::uuids::uuid uuid) override;

 private:
  userver::clients::http::Client& http_client_;
  Aws::S3::S3Client s3_client_;
};

}  // namespace repository
