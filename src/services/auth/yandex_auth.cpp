#include "yandex_auth.hpp"

#include <set>

#include "../../repository/user_data_repository.hpp"

namespace {

services::AuthData MapToResponse(repository::AuthData&& auth_data) {
  std::set<services::Permission> permissions;
  for (const auto& permission : auth_data.permission_group) {
    if (permission == "moderator") {
      permissions.emplace(services::Permission::kModerateRequests);
    } else if (permission == "admin") {
      permissions.emplace(services::Permission::kManageModerators);
    }
  }

  return services::AuthData{auth_data.user_id, auth_data.login, auth_data.name,
                            permissions};
}

}  // namespace

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
    return MapToResponse(std::move(db_user_opt.value()));
  }
  auto yandex_user_opt =
      repository_factory.GetHttpRepository()->GetUserData(token);
  if (!yandex_user_opt) {
    return std::nullopt;
  }
  repository_factory.GetDbRepository()->SaveUserData(yandex_user_opt.value());
  return MapToResponse(std::move(yandex_user_opt.value()));
}

}  // namespace services
