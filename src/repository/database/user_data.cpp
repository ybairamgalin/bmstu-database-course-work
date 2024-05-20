#include "user_data.hpp"
#include "../../utils/merge_utils.hpp"
#include "../http/user_data.hpp"

namespace {

struct DbUser {
  int64_t yandex_id;
  std::string token;
  std::string login;
  std::string name;
  std::string phone;
};

repository::AuthData MapToResponse(DbUser&& user,
                                   std::set<std::string>&& permission_groups) {
  return repository::AuthData{
      user.yandex_id,       std::move(user.token), std::move(user.login),
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
  return GetUserDataByQuery(
      "select yandex_id, token, login, name, phone "
      "from service.users "
      "where token = $1",
      token);
}

std::optional<AuthData> DbUserDataRepository::GetUserDataByLogin(
    const std::string& login) {
  return GetUserDataByQuery(
      "select yandex_id, token, login, name, phone "
      "from service.users "
      "where login = $1",
      login);
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

  auto trx = cluster_ptr_->Begin(
      userver::storages::postgres::ClusterHostType::kMaster, {});
  const auto db_permissions = trx.Execute(
      "select slug "
      "from service.permissions "
      "where user_id = $1",
      auth_data.user_id);
  const auto permissions_set =
      db_permissions.AsContainer<std::set<std::string>>();
  const auto merged_result =
      utils::Merge(permissions_set, auth_data.permission_group);
  LOG_ERROR() << merged_result.added.size();
  LOG_ERROR() << merged_result.deleted.size();
  LOG_ERROR() << auth_data.permission_group.size();
  LOG_ERROR() << permissions_set.size();
  if (!merged_result.added.empty()) {
    trx.Execute(
        "insert into service.permissions (slug, user_id) "
        "(select unnest($1), $2)",
        merged_result.added, auth_data.user_id);
  }
  if (!merged_result.deleted.empty()) {
    trx.Execute(
        "delete from service.permissions "
        "where slug in (select unnest($1)) and user_id = $2",
        merged_result.deleted, auth_data.user_id);
  }
  trx.Commit();
}

template <typename... Args>
std::optional<repository::AuthData> DbUserDataRepository::GetUserDataByQuery(
    const std::string& query, Args&&... args) {
  auto result_user = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query,
      std::forward<Args>(args)...);
  auto db_user = result_user.template AsOptionalSingleRow<::DbUser>(
      userver::storages::postgres::kRowTag);
  if (!db_user) {
    return std::nullopt;
  }
  auto permissions = GetPermissions(db_user->yandex_id);

  return MapToResponse(std::move(db_user.value()), std::move(permissions));
}

std::set<std::string> DbUserDataRepository::GetPermissions(int64_t user_id) {
  auto result_permissions = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select slug "
      "from service.permissions "
      "where user_id = $1",
      user_id);
  return result_permissions.AsContainer<std::set<std::string>>();
}

}  // namespace repository