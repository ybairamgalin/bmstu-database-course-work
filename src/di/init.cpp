#include "init.hpp"

#include <memory>
#include <string>

#include <aws/core/client/ClientConfiguration.h>

#include <aws/s3/S3Client.h>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
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

  Aws::Client::ClientConfiguration config;
  config.region = Aws::String("ru-central1");
  config.endpointOverride = Aws::String("storage.yandexcloud.net");
  auto s3_client = std::make_shared<Aws::S3::S3Client>(config);

  auto repository_factory =
      std::make_unique<repository::SimpleRepositoryFactory>(
          http_client, cluster_ptr, s3_client);
  return std::make_unique<services::SimpleServiceFactory>(
      std::move(repository_factory));
}

}  // namespace di
