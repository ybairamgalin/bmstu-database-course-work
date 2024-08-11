#include "request_comment_post.hpp"

#include "utils/auth.hpp"

namespace handlers::v2 {

RequestCommentPost::RequestCommentPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestCommentPost::Response RequestCommentPost::HandleJson(
    RequestCommentPost::Request&& request,
    userver::server::request::RequestContext&) const {
  auto auth_data =
      utils::AuthOrThrow(request.headers, services_->MakeAuthService());
  auto request_id = http::GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
      request.query_params, "request_id");

  services_->MakeRequestManagementService()->AddComment(
      request_id, request.body.content, auth_data.user_id);
  return RequestCommentPost::Response{201, {}};
}

}  // namespace handlers::v2
