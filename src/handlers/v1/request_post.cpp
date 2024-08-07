#include "request_post.hpp"

#include <gen/handlers/request_post.hpp>

#include <userver/formats/json.hpp>

#include "../../services/auth/yandex_auth.hpp"
#include "utils/uuid.hpp"

namespace handlers {

RequestPost::RequestPost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseHandlerWithAuth(config, context) {}

userver::formats::json::Value RequestPost::Handle(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    const services::AuthData& user_data) const {
  auto req = gen::Parse(
      request_json, userver::formats::parse::To<gen::RequestPostBody>());
  userver::formats::json::ValueBuilder builder;

  services::Request mapped_request{};

  if (!request_json.HasMember("event_id") ||
      !request_json["event_id"].IsInt()) {
    request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    builder["message"] = "Event id should be specified";
  }
//  mapped_request.event_id = request_json["event_id"].As<int64_t>();

  if (request_json.HasMember("attachment_ids") &&
      request_json["attachment_ids"].IsArray()) {
    for (const auto& attachment_id : request_json["attachment_ids"]) {
      if (!attachment_id.IsString()) {
        continue;
      }
      auto uuid_opt = utils::ToUuid(attachment_id.As<std::string>());
      if (!uuid_opt) {
        continue;
      }
      mapped_request.attachment_ids.emplace_back(uuid_opt.value());
    }
  }

  if (request_json.HasMember("id") && request_json["id"].IsInt()) {
//    mapped_request.request_id = request_json["id"].As<int64_t>();
//    service_factory_->MakeRequestManagementService()->UpdateRequest(
//        mapped_request);
  } else {
//    service_factory_->MakeRequestManagementService()->AddRequest(
//        mapped_request);
  }

  return builder.ExtractValue();
}

}  // namespace handlers
