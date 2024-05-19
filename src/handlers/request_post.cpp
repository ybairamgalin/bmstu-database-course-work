#include "request_post.hpp"

#include <userver/formats/json.hpp>

namespace handlers {

RequestPost::RequestPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      cluster_ptr_(
          component_context
              .FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()) {}

userver::formats::json::Value RequestPost::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext& context) const {
  userver::formats::json::ValueBuilder json_value_builder;
  json_value_builder["code"] = 200;
  json_value_builder["message"] = "Good response";
  return json_value_builder.ExtractValue();
}

}  // namespace handlers
