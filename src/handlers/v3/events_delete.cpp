#include "events_delete.hpp"

#include <http/query.hpp>
#include <services/exception.hpp>

#include <utils/uuid.hpp>

namespace handlers::v3 {

EventsDelete::EventsDelete(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

EventsDelete::Response EventsDelete::HandleJson(
    EventsDelete::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  auto uuid_opt = utils::ToUuid(request.path_args.front());
  if (!uuid_opt) {
    throw services::ServiceLevelException("Bad uuid",
                                          services::ErrorType::kInvalidInput);
  }
  services_->MakeEventService()->DeleteEvent(uuid_opt.value(), auth);
  return EventsDelete::Response{204, {}};
}

}  // namespace handlers::v3
