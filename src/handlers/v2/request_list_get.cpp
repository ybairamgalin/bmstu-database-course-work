#include "request_list_get.hpp"

namespace handlers::v2 {

RequestListGet::RequestListGet(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestListGet::Response RequestListGet::HandleJson(
    RequestListGet::Request&&,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  auto requests = services_->MakeRequestManagementService()->GetAll(auth);

  gen::RequestListGetResponse200 response{};
  response.requests.reserve(requests.size());
  for (const auto& request : requests) {
    response.requests.emplace_back(gen::RequestListGetResponse200::RequestsA{
        request.id,
        gen::UserFullInfo{
            request.author.id,
            request.author.name,
            request.author.login,
            request.author.phone,
        },
        request.created_at,
    });
  }
  return RequestListGet::Response{response, 200, {}};
}

}  // namespace handlers::v2
