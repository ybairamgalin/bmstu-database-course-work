#pragma once

#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include "../event_repository.hpp"

namespace repository {

class DbEventRepository : public EventRepository {
 public:
  explicit DbEventRepository(userver::storages::postgres::ClusterPtr cluster_ptr);

  std::vector<Event> GetEventsByIds(
      const std::vector<boost::uuids::uuid>& event_id) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
