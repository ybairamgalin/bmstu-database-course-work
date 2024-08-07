#include "factory.hpp"

#include <memory>

#include "database/event.hpp"
#include "database/file_meta.hpp"
#include "database/request.hpp"
#include "database/user_data.hpp"
#include "http/file_storage.hpp"
#include "http/user_data.hpp"

namespace repository {

SimpleRepositoryFactory::SimpleRepositoryFactory(
    userver::clients::http::Client& http_client,
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : http_client_(http_client), cluster_ptr_(std::move(cluster_ptr)) {}

std::unique_ptr<RequestsRepository>
SimpleRepositoryFactory::MakeRequestsRepository() {
  return std::make_unique<DbRequestsRepository>(cluster_ptr_);
}

std::unique_ptr<UserDataRepository>
SimpleRepositoryFactory::MakeUserDataHttpRepository() {
  return std::make_unique<YandexLoginDataProvider>(http_client_);
}

std::unique_ptr<UserDataRepository>
SimpleRepositoryFactory::MakeUserDataDbRepository() {
  return std::make_unique<DbUserDataRepository>(cluster_ptr_);
}

std::unique_ptr<FileMetaRepository>
SimpleRepositoryFactory::MakeFileMetaRepository() {
  return std::make_unique<DbMetaRepository>(cluster_ptr_);
}

std::unique_ptr<FileStorageRepository>
SimpleRepositoryFactory::MakeFileStorageRepository() {
  return std::make_unique<YandexS3FileStorage>(http_client_);
}

std::unique_ptr<EventRepository>
SimpleRepositoryFactory::MakeEventsRepository() {
  return std::make_unique<DbEventRepository>(cluster_ptr_);
}

}  // namespace repository
