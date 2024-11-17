#pragma once

#include "gen/common.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v3 {

class EventsIdGet : public BaseJsonHandler<http::EmptyRequestBody, gen::Event> {
 public:
  static constexpr std::string_view kName = "v3_events_id_get";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  EventsIdGet(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
