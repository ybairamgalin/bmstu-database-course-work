#pragma once

#include "gen/handlers/event_post.hpp"
#include "handler.hpp"
#include "http/request.hpp"

namespace handlers::v2 {

class EventPost
    : public BaseHandler<gen::EventPostBody, gen::EventPostResponse201> {
 public:
  static constexpr std::string_view kName = "v2_event_post";

  using BaseHandler<gen::EventPostBody, gen::EventPostResponse201>::Request;
  using BaseHandler<gen::EventPostBody, gen::EventPostResponse201>::Response;

  EventPost(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
