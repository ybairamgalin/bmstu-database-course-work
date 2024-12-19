#include "events_id_get.hpp"

#include <http/query.hpp>
#include <services/exception.hpp>

#include <utils/uuid.hpp>

namespace handlers::v3 {

EventsIdGet::EventsIdGet(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventsIdGet::Response EventsIdGet::HandleJson(
    EventsIdGet::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  auto uuid_opt = utils::ToUuid(request.path_args.front());
  if (!uuid_opt) {
    throw services::ServiceLevelException("Bad uuid",
                                          services::ErrorType::kInvalidInput);
  }
  const auto event =
      services_->MakeEventService()->GetEventById(uuid_opt.value());
  if (!event.has_value()) {
    throw services::ServiceLevelException("Event does not exist");
  }
  return EventsIdGet::Response{
      gen::Event{event->id, event->name, event->description}, 200, {}};
}

}  // namespace handlers::v3
