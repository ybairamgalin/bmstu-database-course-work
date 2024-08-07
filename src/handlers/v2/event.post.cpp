#include "event_post.hpp"

namespace handlers::v2 {

EventPost::EventPost(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
    : BaseHandler<gen::EventPostBody, gen::EventPostResponse201>(config,
                                                                 context) {}

EventPost::Response EventPost::Handle(EventPost::Request&& request) const {
  auto id = services_->MakeEventService()->AddEvent(
      services::Event{request.body.name, request.body.description});
  return EventPost::Response{gen::EventPostResponse201{id}, 201, {}};
}

}  // namespace handlers::v2
