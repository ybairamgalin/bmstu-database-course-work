#include "init.hpp"

#include <memory>
#include <string>

#include <aws/core/client/ClientConfiguration.h>

#include <aws/s3/S3Client.h>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/storages/mongo/pool.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/yaml_config/yaml_config.hpp>
#include <utility>

#include "repository/factory.hpp"

namespace di {

struct DiComponent::Impl {
  Impl(userver::clients::http::Client& http_client,
       userver::storages::postgres::ClusterPtr cluster_ptr,
       userver::storages::mongo::PoolPtr mongo_pool,
       const userver::components::ComponentConfig& cfg)
      : http_client_(http_client),
        cluster_ptr_(std::move(cluster_ptr)),
        mongo_pool_(std::move(mongo_pool)),
        should_use_mongo(cfg["use-mongo"].As<bool>()) {
    Aws::Client::ClientConfiguration config;
    config.region = Aws::String(cfg["s3-region"].As<std::string>());
    config.endpointOverride = Aws::String(cfg["s3-endpoint"].As<std::string>());
    s3_client_ = std::make_shared<Aws::S3::S3Client>(config);
  }

  userver::clients::http::Client& http_client_;
  userver::storages::postgres::ClusterPtr cluster_ptr_;
  userver::storages::mongo::PoolPtr mongo_pool_;
  bool should_use_mongo;

  std::shared_ptr<Aws::S3::S3Client> s3_client_;
};

DiComponent::DiComponent(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      impl_(std::make_unique<Impl>(
          context.FindComponent<userver::components::HttpClient>()
              .GetHttpClient(),
          context
              .FindComponent<userver::components::Postgres>(
                  config["pg-component"].As<std::string>())
              .GetCluster(),
          context
              .FindComponent<userver::components::Mongo>(
                  config["mongo-component"].As<std::string>())
              .GetPool(),
          config)) {}

std::unique_ptr<services::IServiceFactory> DiComponent::MakeServiceFactory() {
  std::unique_ptr<repository::IRepositoryFactory> repository_factory;
  if (impl_->should_use_mongo) {
    repository_factory =
        std::make_unique<repository::RequestMongoRepositoryFactory>(
            impl_->http_client_, impl_->cluster_ptr_, impl_->mongo_pool_,
            impl_->s3_client_);
  } else {
    repository_factory = std::make_unique<repository::SimpleRepositoryFactory>(
        impl_->http_client_, impl_->cluster_ptr_, impl_->s3_client_);
  }
  return std::make_unique<services::SimpleServiceFactory>(
      std::move(repository_factory));
}

[[maybe_unused]] userver::yaml_config::Schema
DiComponent::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(
      R"(
type: object
description: di component
additionalProperties: false
properties:
    pg-component:
        type: string
        description: Pg-component
    mongo-component:
        type: string
        description: Component
    s3-region:
        type: string
        description: Component
    s3-endpoint:
        type: string
        description: Component
    use-mongo:
        type: boolean
        description: Component
)");
}

DiComponent::~DiComponent() = default;

}  // namespace di
