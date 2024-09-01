#include "request_comment_post.hpp"

#include "utils/auth.hpp"

namespace handlers::v2 {

RequestCommentPost::RequestCommentPost(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

RequestCommentPost::Response RequestCommentPost::HandleJson(
    RequestCommentPost::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");

  auto request_id = http::GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
      request.query_params, "request_id");

  services_->MakeRequestManagementService()->AddComment(
      request_id, request.body.content, auth);
  return RequestCommentPost::Response{201, {}};
}

}  // namespace handlers::v2
