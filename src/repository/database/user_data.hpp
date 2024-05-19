#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "../user_data_repository.hpp"

namespace repository {

class DbUserDataRepository : public UserDataRepository {
 public:
  DbUserDataRepository(userver::storages::postgres::ClusterPtr cluster_ptr);

  std::optional<repository::AuthData> GetUserData(
      const std::string& token) override;

  void SaveUserData(const repository::AuthData&) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
