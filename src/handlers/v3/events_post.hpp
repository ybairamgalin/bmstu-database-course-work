#pragma once

#include "gen/handlers/event_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v3 {

class EventsPost
    : public BaseJsonHandler<gen::EventPostBody, gen::EventPostResponse201> {
 public:
  static constexpr std::string_view kName = "v3_events_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  EventsPost(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
