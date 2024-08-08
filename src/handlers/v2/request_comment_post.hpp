#pragma once

#include "gen/handlers/request_comment_post.hpp"
#include "handler.hpp"
#include "http/query.hpp"
#include "http/response.hpp"

namespace handlers::v2 {

class RequestCommentPost
    : public BaseHandler<gen::RequestCommentPostBody, http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_request_comment_post";

  using BaseHandler<gen::RequestCommentPostBody,
                    http::EmptyResponseBody>::Request;
  using BaseHandler<gen::RequestCommentPostBody,
                    http::EmptyResponseBody>::Response;

  RequestCommentPost(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
