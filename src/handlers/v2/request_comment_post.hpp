#pragma once

#include "gen/handlers/request_comment_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/query.hpp"
#include "http/response.hpp"

namespace handlers::v2 {

class RequestCommentPost : public BaseJsonHandler<gen::RequestCommentPostBody,
                                                  http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_request_comment_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RequestCommentPost(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
