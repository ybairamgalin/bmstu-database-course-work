#include "event_list_get.hpp"

namespace handlers::v2 {

EventListGet::EventListGet(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventListGet::Response EventListGet::HandleJson(
    EventListGet::Request&&, userver::server::request::RequestContext&) const {
  auto events = services_->MakeEventService()->GetAllEvents();
  gen::EventListGet200Response response{};
  response.events.reserve(events.size());
  for (auto& event : events) {
    response.events.emplace_back(gen::Event{event.id, std::move(event.name),
                                            std::move(event.description)});
  }
  return EventListGet::Response{response, 200, {}};
}

}  // namespace handlers::v2
