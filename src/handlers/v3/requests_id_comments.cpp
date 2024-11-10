#include "requests_id_comments.hpp"

#include "utils/auth.hpp"
#include "utils/uuid.hpp"

namespace handlers::v3 {

RequestsIdCommentsPost::RequestsIdCommentsPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestsIdCommentsPost::Response RequestsIdCommentsPost::HandleJson(
    RequestsIdCommentsPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");
  auto request_id = utils::ToUuid(request.path_args.front());
  if (!request_id) {
    throw http::HttpException(400, "Invalid request id");
  }

  services_->MakeRequestManagementService()->AddComment(
      request_id.value(), request.body.content, auth);
  return RequestsIdCommentsPost::Response{201, {}};
}

}  // namespace handlers::v3
