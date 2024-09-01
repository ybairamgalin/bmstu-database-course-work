#pragma once

#include <userver/storages/postgres/cluster.hpp>

#include "../requests_repository.hpp"

namespace repository {

class DbRequestsRepository : public RequestsRepository {
 public:
  explicit DbRequestsRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);
  std::optional<RequestFull> GetRequestById(
      const boost::uuids::uuid& uuid) override;
  void Insert(const repository::Request& request) override;
  void Update(const repository::Request& request) override;
  void AddComment(const boost::uuids::uuid& id, const std::string& content,
                  int64_t author_id) override;
  std::vector<RequestShort> GetAll() override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
