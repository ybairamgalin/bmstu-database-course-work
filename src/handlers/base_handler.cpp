#include "base_handler.hpp"

#include "../services/auth/yandex_auth.hpp"

namespace handlers {

BaseHandlerWithAuth::BaseHandlerWithAuth(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      http_client_(context.FindComponent<userver::components::HttpClient>()
                       .GetHttpClient()),
      cluster_ptr_(
          context.FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()) {}

userver::formats::json::Value BaseHandlerWithAuth::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  userver::formats::json::ValueBuilder response_builder;
  if (!request.HasHeader("Token")) {
    response_builder["message"] = "Token was not provided";
    return response_builder.ExtractValue();
  }
  auto auth_service = services::YandexAuthService{http_client_, cluster_ptr_};
  const auto& token = request.GetHeader("Token");
  auto auth_data = auth_service.GetAuthDataByToken(token);
  if (!auth_data) {
    response_builder["message"] = "Failed to auth";
    return response_builder.ExtractValue();
  }

  return Handle(request, request_json, auth_data.value());
}

}  // namespace handlers
