#pragma once

#include "gen/handlers/manage_access_put.hpp"
#include "handlers/base_json_handler.hpp"

namespace handlers::v3 {

class RolesPut : public BaseJsonHandler<gen::ManageAccessPutBody,
                                        http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v3_roles_put";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RolesPut(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
