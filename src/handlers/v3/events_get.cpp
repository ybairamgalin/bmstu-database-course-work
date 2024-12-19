#include "events_get.hpp"

#include <http/query.hpp>

namespace handlers::v3 {

EventsGet::EventsGet(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventsGet::Response EventsGet::HandleJson(
    EventsGet::Request&& request,
    userver::server::request::RequestContext&) const {
  const auto name_part =
      http::GetQueryParamOpt<std::string>(request.query_params, "name_part");

  std::string substring;
  if (name_part.has_value()) {
    substring = name_part.value();
  }

  auto events = services_->MakeEventService()->SearchEvents(substring);
  gen::EventListGet200Response response{};
  response.events.reserve(events.size());
  for (auto& event : events) {
    response.events.emplace_back(gen::Event{event.id, std::move(event.name),
                                            std::move(event.description)});
  }
  return EventsGet::Response{response, 200, {}};
}

}  // namespace handlers::v3
