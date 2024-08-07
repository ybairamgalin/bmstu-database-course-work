#include "event.hpp"

#include <userver/storages/postgres/cluster_types.hpp>
#include <utility>

namespace repository {

DbEventRepository::DbEventRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

std::vector<Event> DbEventRepository::GetEventsByIds(
    const std::vector<boost::uuids::uuid>& event_ids) {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "select event_id, name "
      "from service.events "
      "where event_id in (select unnest($1))",
      event_ids);
  return result.AsContainer<std::vector<Event>>(
      userver::storages::postgres::kRowTag);
}

}  // namespace repository
