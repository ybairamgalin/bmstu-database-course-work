#include "file_storage.hpp"

namespace repository {

void YandexS3FileStorage::UploadFile(std::string&& file_content,
                                     boost::uuids::uuid uuid) {}

std::optional<std::string> YandexS3FileStorage::GetFile(
    boost::uuids::uuid uuid) {
  return std::string();
}

YandexS3FileStorage::YandexS3FileStorage(
    userver::clients::http::Client& http_client)
    : http_client_(http_client) {
  http_client_.CreateRequest().put()
}
}  // namespace repository
