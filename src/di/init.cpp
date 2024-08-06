#include "init.hpp"

#include <memory>
#include <string>

#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json_fwd.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace di {

std::unique_ptr<services::IServiceFactory> InitFactories(
    const userver::components::ComponentConfig&,
    const userver::components::ComponentContext& context) {
  auto& http_client =
      context.FindComponent<userver::components::HttpClient>().GetHttpClient();
  auto cluster_ptr =
      context.FindComponent<userver::components::Postgres>("postgres-db-1")
          .GetCluster();

  auto repository_factory =
      std::make_unique<repository::SimpleRepositoryFactory>(http_client,
                                                            cluster_ptr);
  return std::make_unique<services::SimpleServiceFactory>(
      std::move(repository_factory));
}

}
