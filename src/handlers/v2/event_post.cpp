#include "event_post.hpp"

namespace handlers::v2 {

EventPost::EventPost(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventPost::Response EventPost::HandleJson(
    EventPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto id = services_->MakeEventService()->AddEvent(
      services::Event{request.body.name, request.body.description});
  return EventPost::Response{gen::EventPostResponse201{id}, 201, {}};
}

}  // namespace handlers::v2
