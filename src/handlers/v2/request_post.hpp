#pragma once

#include "gen/handlers/request_post.hpp"
#include "handler.hpp"

namespace handlers::v2 {

class RequestPost
    : public BaseHandler<gen::RequestPostBody, http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_request_post";

  using BaseHandler<gen::RequestPostBody, http::EmptyResponseBody>::Request;
  using BaseHandler<gen::RequestPostBody, http::EmptyResponseBody>::Response;

  RequestPost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
