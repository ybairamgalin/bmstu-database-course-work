#include "factory.hpp"

#include <aws/s3/S3Client.h>

#include <memory>

#include "database/event.hpp"
#include "database/file_meta.hpp"
#include "database/mongo_request.hpp"
#include "database/request.hpp"
#include "database/user_data.hpp"
#include "http/file_storage.hpp"
#include "http/user_data.hpp"

namespace repository {

struct SimpleRepositoryFactory::Impl {
  Impl(userver::clients::http::Client& http_client,
       userver::storages::postgres::ClusterPtr cluster_ptr,
       std::shared_ptr<Aws::S3::S3Client> s3_client)
      : http_client(http_client),
        cluster_ptr(cluster_ptr),
        s3_client(s3_client) {}

  userver::clients::http::Client& http_client;
  userver::storages::postgres::ClusterPtr cluster_ptr;
  std::shared_ptr<Aws::S3::S3Client> s3_client;
};

SimpleRepositoryFactory::SimpleRepositoryFactory(
    userver::clients::http::Client& http_client,
    userver::storages::postgres::ClusterPtr cluster_ptr,
    std::shared_ptr<Aws::S3::S3Client> s3_client)
    : impl_(std::make_unique<Impl>(
          Impl(http_client, std::move(cluster_ptr), std::move(s3_client)))) {}

std::unique_ptr<RequestsRepository>
SimpleRepositoryFactory::MakeRequestsRepository() {
  return std::make_unique<DbRequestsRepository>(impl_->cluster_ptr);
}

std::unique_ptr<UserDataRepository>
SimpleRepositoryFactory::MakeUserDataHttpRepository() {
  return std::make_unique<YandexLoginDataProvider>(impl_->http_client);
}

std::unique_ptr<UserDataRepository>
SimpleRepositoryFactory::MakeUserDataDbRepository() {
  return std::make_unique<DbUserDataRepository>(impl_->cluster_ptr);
}

std::unique_ptr<FileMetaRepository>
SimpleRepositoryFactory::MakeFileMetaRepository() {
  return std::make_unique<DbMetaRepository>(impl_->cluster_ptr);
}

std::unique_ptr<FileStorageRepository>
SimpleRepositoryFactory::MakeFileStorageRepository() {
  return std::make_unique<YandexS3FileStorage>(impl_->s3_client);
}

std::unique_ptr<EventRepository>
SimpleRepositoryFactory::MakeEventsRepository() {
  return std::make_unique<DbEventRepository>(impl_->cluster_ptr);
}

SimpleRepositoryFactory::~SimpleRepositoryFactory() = default;

RequestMongoRepositoryFactory::RequestMongoRepositoryFactory(
    userver::clients::http::Client& http_client,
    userver::storages::postgres::ClusterPtr cluster_ptr,
    userver::storages::mongo::PoolPtr mongo_pool,
    std::shared_ptr<Aws::S3::S3Client> s3_client)
    : SimpleRepositoryFactory(http_client, std::move(cluster_ptr),
                              std::move(s3_client)),
      mongo_pool_(std::move(mongo_pool)) {}

RequestMongoRepositoryFactory::~RequestMongoRepositoryFactory() = default;

std::unique_ptr<RequestsRepository>
RequestMongoRepositoryFactory::MakeRequestsRepository() {
  return std::make_unique<MongoRequestRepository>(mongo_pool_);
}

}  // namespace repository
