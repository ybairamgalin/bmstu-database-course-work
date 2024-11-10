#include "events_post.hpp"

namespace handlers::v3 {

EventsPost::EventsPost(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventsPost::Response EventsPost::HandleJson(
    EventsPost::Request&& request,
    userver::server::request::RequestContext&) const {
  auto id = services_->MakeEventService()->AddEvent(
      services::Event{request.body.name, request.body.description});
  return EventsPost::Response{gen::EventPostResponse201{id}, 201, {}};
}

}  // namespace handlers::v3
