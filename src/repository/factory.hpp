#pragma once

#include <memory>

#include <userver/storages/mongo/pool.hpp>

#include "event_repository.hpp"
#include "file_meta_repository.hpp"
#include "file_storage_repository.hpp"
#include "requests_repository.hpp"
#include "user_data_repository.hpp"

namespace Aws::S3 {
class S3Client;
}

namespace repository {

class IRepositoryFactory {
 public:
  virtual ~IRepositoryFactory() = default;
  virtual std::unique_ptr<RequestsRepository> MakeRequestsRepository() = 0;
  virtual std::unique_ptr<UserDataRepository> MakeUserDataDbRepository() = 0;
  virtual std::unique_ptr<UserDataRepository> MakeUserDataHttpRepository() = 0;
  virtual std::unique_ptr<FileMetaRepository> MakeFileMetaRepository() = 0;
  virtual std::unique_ptr<EventRepository> MakeEventsRepository() = 0;
  virtual std::unique_ptr<FileStorageRepository>
  MakeFileStorageRepository() = 0;
};

class SimpleRepositoryFactory : public IRepositoryFactory {
 public:
  SimpleRepositoryFactory(userver::clients::http::Client& http_client,
                          userver::storages::postgres::ClusterPtr cluster_ptr,
                          std::shared_ptr<Aws::S3::S3Client> s3_client);
  ~SimpleRepositoryFactory() override;

  std::unique_ptr<RequestsRepository> MakeRequestsRepository() override;
  std::unique_ptr<UserDataRepository> MakeUserDataDbRepository() override;
  std::unique_ptr<UserDataRepository> MakeUserDataHttpRepository() override;
  std::unique_ptr<FileMetaRepository> MakeFileMetaRepository() override;
  std::unique_ptr<FileStorageRepository> MakeFileStorageRepository() override;
  std::unique_ptr<EventRepository> MakeEventsRepository() override;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

class RequestMongoRepositoryFactory : public SimpleRepositoryFactory {
 public:
  RequestMongoRepositoryFactory(
      userver::clients::http::Client& http_client,
      userver::storages::postgres::ClusterPtr cluster_ptr,
      userver::storages::mongo::PoolPtr mongo_pool,
      std::shared_ptr<Aws::S3::S3Client> s3_client);
  ~RequestMongoRepositoryFactory() override;

  std::unique_ptr<RequestsRepository> MakeRequestsRepository() override;

 private:
  userver::storages::mongo::PoolPtr mongo_pool_;
};

}  // namespace repository
