#include "request_get.hpp"

#include <http/query.hpp>
#include "gen/handlers/request_get.hpp"

namespace handlers::v2 {

RequestGet::RequestGet(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : BaseHandler<http::EmptyRequestBody, gen::RequestGetResponse200>(
          config, context) {}

RequestGet::Response RequestGet::Handle(RequestGet::Request&& request) const {
  auto request_id = http::GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
      request.query_params, "request_id");

  auto request_full =
      services_->MakeRequestManagementService()->GetRequestById(request_id);

  return RequestGet::Response{
      gen::RequestGetResponse200{
          request_full.event_id,
          {},
          gen::UserFullInfo{
              request_full.author.id,
              request_full.author.name,
              request_full.author.login,
              request_full.author.phone,
          },
          request_full.description,
          request_full.created_at,
          request_full.updated_at,
      },
      200,
      {},
  };
}

}  // namespace handlers::v2
