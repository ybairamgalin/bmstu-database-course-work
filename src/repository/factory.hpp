#pragma once

#include <memory>

#include "file_meta_repository.hpp"
#include "file_storage_repository.hpp"
#include "requests_repository.hpp"
#include "user_data_repository.hpp"

namespace repository {

class IRepositoryFactory {
 public:
  virtual ~IRepositoryFactory() = default;
  virtual std::unique_ptr<RequestsRepository> MakeRequestsRepository() = 0;
  virtual std::unique_ptr<UserDataRepository> MakeUserDataDbRepository() = 0;
  virtual std::unique_ptr<UserDataRepository> MakeUserDataHttpRepository() = 0;
  virtual std::unique_ptr<FileMetaRepository> MakeFileMetaRepository() = 0;
  virtual std::unique_ptr<FileStorageRepository>
  MakeFileStorageRepository() = 0;
};

class SimpleRepositoryFactory : public IRepositoryFactory {
 public:
  SimpleRepositoryFactory(userver::clients::http::Client& http_client,
                          userver::storages::postgres::ClusterPtr cluster_ptr);
  std::unique_ptr<RequestsRepository> MakeRequestsRepository() override;
  std::unique_ptr<UserDataRepository> MakeUserDataDbRepository() override;
  std::unique_ptr<UserDataRepository> MakeUserDataHttpRepository() override;
  std::unique_ptr<FileMetaRepository> MakeFileMetaRepository() override;
  std::unique_ptr<FileStorageRepository> MakeFileStorageRepository() override;

 private:
  userver::clients::http::Client& http_client_;
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
