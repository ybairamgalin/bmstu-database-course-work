#include "user_data.hpp"
#include "../http/user_data.hpp"

namespace {

struct DbUser {
  int64_t yandex_id;
  std::string token;
  std::string login;
  std::string name;
  std::string phone;
};

repository::AuthData MapToResponse(
    DbUser&& user, std::vector<std::string>&& permission_groups) {
  return repository::AuthData{
      user.yandex_id,       std::move(user.token),        std::move(user.login),
      std::move(user.name), std::move(user.phone), std::move(permission_groups),
  };
}

}  // namespace

namespace repository {

DbUserDataRepository::DbUserDataRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

std::optional<repository::AuthData> DbUserDataRepository::GetUserData(
    const std::string& token) {
  auto result_user = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select yandex_id, token, login, name, phone "
      "from service.users "
      "where token = $1",
      token);
  auto db_user = result_user.AsOptionalSingleRow<DbUser>(
      userver::storages::postgres::kRowTag);
  if (!db_user) {
    return std::nullopt;
  }

  auto result_permissions = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select slug "
      "from service.permissions "
      "where user_id = $1",
      db_user->yandex_id);
  auto permissions =
      result_permissions.AsContainer<std::vector<std::string>>();

  return MapToResponse(std::move(db_user.value()),
                       std::move(permissions));
}

void DbUserDataRepository::SaveUserData(const repository::AuthData& auth_data) {
  cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "insert into service.users (yandex_id, token, login, name, phone) "
      "values "
      "($1, $2, $3, $4, $5) "
      "on conflict (login) "
      "do update set "
      "token = excluded.token, "
      "name = excluded.name, "
      "phone = excluded.phone ",
      auth_data.user_id, auth_data.token, auth_data.login, auth_data.name,
      auth_data.phone);
}

}  // namespace repository