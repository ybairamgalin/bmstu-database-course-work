#include "user_data_repository.hpp"

#include "database/user_data.hpp"
#include "http/user_data.hpp"

namespace repository {

YandexDataRepositoryFactory::YandexDataRepositoryFactory(
    userver::clients::http::Client& http_client,
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : http_client_(http_client), cluster_ptr_(std::move(cluster_ptr)) {}

std::unique_ptr<UserDataRepository>
YandexDataRepositoryFactory::GetDbRepository() {
  return std::make_unique<DbUserDataRepository>(cluster_ptr_);
}

std::unique_ptr<UserDataRepository>
YandexDataRepositoryFactory::GetHttpRepository() {
  return std::make_unique<YandexLoginDataProvider>(http_client_);
}

}  // namespace repository
