#include "request_post.hpp"

#include <boost/uuid/nil_generator.hpp>

#include "http/query.hpp"
#include "services/request_management_service.hpp"
#include "utils/auth.hpp"

namespace handlers::v2 {

RequestPost::RequestPost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestPost::Response RequestPost::HandleJson(
    RequestPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

  const auto request_id_opt = http::GetQueryParamOpt<boost::uuids::uuid>(
      request.query_params, "request_id");

  services::RequestToCreateOrUpdate request_creation_request{};
  request_creation_request.author = auth;

  if (request.body.description.has_value()) {
    request_creation_request.description = request.body.description.value();
  }
  request_creation_request.event_id = request.body.event_id;

  request_creation_request.attachments.reserve(request.body.attachments.size());
  for (const auto& attachment : request.body.attachments) {
    request_creation_request.attachments.emplace_back(
        services::Attachment{attachment.id, attachment.filename});
  }

  if (request.body.status.has_value()) {
    request_creation_request.status =
        gen::ToString(request.body.status.value());
  }

  if (request_id_opt.has_value()) {
    services_->MakeRequestManagementService()->UpdateRequest(
        request_id_opt.value(), request_creation_request);
    return Response{
        gen::RequestPostResponse200{request_id_opt.value()}, 200, {}};
  }

  auto uuid = services_->MakeRequestManagementService()->AddRequest(
      request_creation_request);
  return Response{gen::RequestPostResponse200{uuid}, 201, {}};
}

}  // namespace handlers::v2
