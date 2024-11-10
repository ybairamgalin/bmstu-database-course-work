#include "requests_get.hpp"

#include "http/query.hpp"

namespace handlers::v3 {

RequestsGet::RequestsGet(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestsGet::Response RequestsGet::HandleJson(
    RequestsGet::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  services::RequestFilters filters;
  filters.author_login =
      http::GetQueryParamOpt<std::string>(request.query_params, "author_login");
  filters.request_id = http::GetQueryParamOpt<boost::uuids::uuid>(
      request.query_params, "request_id");
  filters.event_id = http::GetQueryParamOpt<boost::uuids::uuid>(
      request.query_params, "event_id");
  filters.created_after =
      http::GetQueryParamOpt<userver::utils::datetime::TimePointTz>(
          request.query_params, "created_after");

  auto requests =
      services_->MakeRequestManagementService()->GetFiltered(filters, auth);

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
  return RequestsGet::Response{response, 200, {}};
}

}  // namespace handlers::v3
