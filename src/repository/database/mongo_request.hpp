#pragma once

#include <userver/storages/mongo/pool.hpp>

#include "repository/requests_repository.hpp"

namespace repository {

class MongoRequestRepository : public RequestsRepository {
 public:
  explicit MongoRequestRepository(
      userver::storages::mongo::PoolPtr  pool);

  std::optional<RequestFull> GetRequestById(
      const boost::uuids::uuid& uuid) override;
  std::vector<RequestShort> GetAll() override;
  void Insert(const repository::Request& request) override;
  void Update(const repository::Request& request) override;
  void AddComment(const boost::uuids::uuid& id, const std::string& content,
                  int64_t author_id) override;

 private:
  userver::storages::mongo::PoolPtr pool_;
};

}  // namespace repository
