#include "requests_post.hpp"
#include <boost/uuid/nil_generator.hpp>

#include "http/query.hpp"
#include "services/request_management_service.hpp"
#include "utils/auth.hpp"

namespace handlers::v3 {

RequestsPost::RequestsPost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestsPost::Response RequestsPost::HandleJson(
    RequestsPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth = ctx.GetData<services::AuthData>("auth");

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

  auto uuid = services_->MakeRequestManagementService()->AddRequest(
      request_creation_request);
  return Response{gen::RequestPostResponse200{uuid}, 201, {}};
}

}  // namespace handlers::v3
