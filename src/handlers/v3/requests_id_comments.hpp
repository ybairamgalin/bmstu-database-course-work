#pragma once

#include "gen/handlers/request_comment_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/query.hpp"
#include "http/response.hpp"

namespace handlers::v3 {

class RequestsIdCommentsPost
    : public BaseJsonHandler<gen::RequestCommentPostBody,
                             http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v3_requests_id_comments";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RequestsIdCommentsPost(const userver::components::ComponentConfig& config,
                        const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
