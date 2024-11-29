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
      "select event_id, name, description "
      "from service.events "
      "where event_id in (select unnest($1))",
      event_ids);
  return result.AsContainer<std::vector<Event>>(
      userver::storages::postgres::kRowTag);
}

void DbEventRepository::AddEvent(const Event& event) {
  LOG_WARNING() << "Adding event";
  cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "insert into service.events (event_id, name, description) "
      "values ($1, $2, $3) "
      "on conflict (event_id) do update "
      "set name = excluded.name, description = excluded.description",
      event.uuid, event.name, event.description);
}

std::vector<Event> DbEventRepository::GetAll() {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kSlaveOrMaster,
      "select event_id, name, description "
      "from service.events ");
  return result.AsContainer<std::vector<Event>>(
      userver::storages::postgres::kRowTag);
}

void DbEventRepository::DeleteEvent(const boost::uuids::uuid& id) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "delete from service.events "
                        "where event_id = $1",
                        id);
}

}  // namespace repository
