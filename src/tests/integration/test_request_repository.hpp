#pragma once

#include <string_view>

#include <userver/components/component.hpp>
#include <userver/components/component_base.hpp>
#include <userver/storages/mongo/pool.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

class RequestRepositoryIntTestComponent
    : public userver::components::ComponentBase {
 public:
  static constexpr std::string_view kName = "request_int_test";

  RequestRepositoryIntTestComponent(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& context);

 private:
  void RunTests();
  void Cleanup();

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
  userver::storages::mongo::PoolPtr pool_;
};
