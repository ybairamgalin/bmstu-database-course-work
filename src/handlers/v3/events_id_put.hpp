#pragma once

#include "gen/handlers/event_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v3 {

class EventsIdPut
    : public BaseJsonHandler<gen::EventPostBody, http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v3_events_id_put";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  EventsIdPut(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
