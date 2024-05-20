#include "yandex_auth.hpp"

#include <set>

#include "../../repository/user_data_repository.hpp"
#include "../mapping/mapping.hpp"

namespace services {

YandexAuthService::YandexAuthService(
    userver::clients::http::Client& http_client_,
    userver::storages::postgres::ClusterPtr cluster_ptr_)
    : http_client_(http_client_), cluster_ptr_(std::move(cluster_ptr_)) {}

std::optional<services::AuthData> YandexAuthService::GetAuthDataByToken(
    const std::string& token) {
  repository::YandexDataRepositoryFactory repository_factory(http_client_,
                                                             cluster_ptr_);
  auto db_user_opt = repository_factory.GetDbRepository()->GetUserData(token);
  if (db_user_opt) {
    return internal::MapAuthData(db_user_opt.value());
  }
  auto yandex_user_opt =
      repository_factory.GetHttpRepository()->GetUserData(token);
  if (!yandex_user_opt) {
    return std::nullopt;
  }
  repository_factory.GetDbRepository()->SaveUserData(yandex_user_opt.value());
  return internal::MapAuthData(yandex_user_opt.value());
}

}  // namespace services
