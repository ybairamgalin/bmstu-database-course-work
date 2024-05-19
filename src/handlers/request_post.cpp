#include "request_post.hpp"

#include <userver/formats/json.hpp>

#include "../services/auth/yandex_auth.hpp"

namespace handlers {

RequestPost::RequestPost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseHandlerWithAuth(config, context) {}

userver::formats::json::Value RequestPost::Handle(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    const services::AuthData& user_data) const {
  userver::formats::json::ValueBuilder builder;
  builder["user_data"]["name"] = user_data.name;

  return builder.ExtractValue();
}

}  // namespace handlers
