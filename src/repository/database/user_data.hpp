#pragma once

#include <string>
#include <vector>

#include <userver/storages/postgres/cluster.hpp>

#include "../user_data_repository.hpp"

namespace repository {

class DbUserDataRepository : public UserDataRepository {
 public:
  explicit DbUserDataRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);

  std::vector<AuthData> GetUserDataByIds(
      const std::vector<int64_t>& ids) override;
  std::optional<repository::AuthData> GetUserData(
      const std::string& token) override;
  std::optional<AuthData> GetUserDataByLogin(const std::string& login) override;

  void SaveUserData(const repository::AuthData&) override;
  void SerUserRole(const std::string& login, const std::string& role) override;
  void AddUserToPermissionGroups(
      int64_t user_id, const std::vector<std::string>& permission_group) override;

 private:
  template <typename... Args>
  std::optional<repository::AuthData> GetUserDataByQuery(
      const std::string& query, Args&&... args);

  std::set<std::string> GetPermissions(int64_t user_id);

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
