#pragma once

#include "gen/handlers/manage_access_put.hpp"
#include "handlers/base_json_handler.hpp"

namespace handlers::v2 {

class ManageAccessPut : public BaseJsonHandler<gen::ManageAccessPutBody,
                                               http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_manage_access_put";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  ManageAccessPut(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
