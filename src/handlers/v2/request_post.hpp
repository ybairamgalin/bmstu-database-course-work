#pragma once

#include "gen/handlers/request_post.hpp"
#include "handler.hpp"

namespace handlers::v2 {

class RequestPost
    : public BaseHandler<gen::RequestPostBody, gen::RequestPostResponse200> {
 public:
  static constexpr std::string_view kName = "v2_request_post";

  using BaseHandler<gen::RequestPostBody, gen::RequestPostResponse200>::Request;
  using BaseHandler<gen::RequestPostBody, gen::RequestPostResponse200>::Response;

  RequestPost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
