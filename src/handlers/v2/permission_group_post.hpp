#pragma once

#include "gen/handlers/permission_group_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/query.hpp"
#include "http/response.hpp"

namespace handlers::v2 {

class PermissionGroupPost : public BaseJsonHandler<gen::PermissionGroupPost,
                                                   http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_permission_group_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  PermissionGroupPost(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
