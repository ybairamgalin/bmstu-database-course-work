#include "request_post.hpp"

#include <boost/uuid/nil_generator.hpp>

#include "services/request_management_service.hpp"
#include "utils/auth.hpp"

namespace handlers::v2 {

RequestPost::RequestPost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestPost::Response RequestPost::HandleJson(
    RequestPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto auth_data =
      utils::AuthOrThrow(request.headers, services_->MakeAuthService());

  services::RequestToCreateOrUpdate request_creation_request{};
  request_creation_request.author_id = auth_data.user_id;

  if (request.body.description.has_value()) {
    request_creation_request.description = request.body.description.value();
  }
  request_creation_request.event_id = request.body.event_id;

  request_creation_request.attachments.reserve(request.body.attachments.size());
  for (const auto& attachment : request.body.attachments) {
    request_creation_request.attachments.emplace_back(
        services::Attachment{attachment.id, attachment.filename});
  }

  auto uuid = services_->MakeRequestManagementService()->AddRequest(
      request_creation_request);

  return Response{gen::RequestPostResponse200{uuid}, 201, {}};
}

}  // namespace handlers::v2
