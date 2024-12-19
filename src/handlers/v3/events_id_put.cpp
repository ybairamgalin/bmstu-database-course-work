#include "events_id_put.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "utils/uuid.hpp"

namespace handlers::v3 {

EventsIdPut::EventsIdPut(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventsIdPut::Response EventsIdPut::HandleJson(
    EventsIdPut::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  auto event_id = utils::ToUuid(request.path_args.front());
  if (!event_id) {
    throw http::HttpException(400, "Invalid request id");
  }

  services_->MakeEventService()->UpdateEvent(
      event_id.value(),
      services::Event{boost::uuids::random_generator()(), request.body.name,
                      request.body.description},
      auth);
  return {200, {}};
}

}  // namespace handlers::v3
