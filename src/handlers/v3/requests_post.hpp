#pragma once

#include "handlers/base_json_handler.hpp"
#include "gen/handlers/request_post.hpp"

namespace handlers::v3 {

class RequestPost : public BaseJsonHandler<gen::RequestPostBody,
                                           gen::RequestPostResponse200> {
 public:
  static constexpr std::string_view kName = "v3_requests_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RequestPost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}