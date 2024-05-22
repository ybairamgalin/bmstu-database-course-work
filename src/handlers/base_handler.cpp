#include "base_handler.hpp"

#include "../services/auth/yandex_auth.hpp"
#include "../services/exception.hpp"

#include <memory>

#include "../repository/factory.hpp"

namespace {

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

}  // namespace

namespace handlers {

BaseHandlerWithAuth::BaseHandlerWithAuth(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      service_factory_(InitFactories(config, context)) {}

userver::formats::json::Value BaseHandlerWithAuth::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  userver::formats::json::ValueBuilder response_builder;
  if (!request.HasHeader("Token")) {
    response_builder["message"] = "Token was not provided";
    return response_builder.ExtractValue();
  }
  const auto& token = request.GetHeader("Token");
  auto auth_service = service_factory_->MakeAuthService();
  auto auth_data =
      service_factory_->MakeAuthService()->GetAuthDataByToken(token);
  if (!auth_data) {
    response_builder["message"] = "Failed to auth";
    return response_builder.ExtractValue();
  }

  try {
    return Handle(request, request_json, auth_data.value());
  } catch (const services::ServiceLevelException& ex) {
    response_builder["message"] = ex.what();
    return response_builder.ExtractValue();
  }
}

}  // namespace handlers
