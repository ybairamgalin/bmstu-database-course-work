#pragma once

#include "gen/handlers/request_post.hpp"
#include "handlers/base_json_handler.hpp"

namespace handlers::v2 {

class RequestPost : public BaseJsonHandler<gen::RequestPostBody,
                                           gen::RequestPostResponse200> {
 public:
  static constexpr std::string_view kName = "v2_request_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RequestPost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
