#pragma once

#include "gen/handlers/event_list_get.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v2 {

class EventListGet : public BaseJsonHandler<http::EmptyRequestBody,
                                           gen::EventListGet200Response> {
 public:
  static constexpr std::string_view kName = "v2_event_list_get";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  EventListGet(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
