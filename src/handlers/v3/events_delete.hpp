#pragma once

#include "gen/handlers/event_list_get.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v3 {

class EventsDelete
    : public BaseJsonHandler<http::EmptyRequestBody, http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v3_events_delete";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  EventsDelete(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
