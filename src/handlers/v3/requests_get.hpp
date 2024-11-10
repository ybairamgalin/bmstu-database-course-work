#pragma once

#include "gen/handlers/request_list_get.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v3 {

class RequestsGet : public BaseJsonHandler<http::EmptyRequestBody,
                                           gen::RequestListGetResponse200> {
 public:
  static constexpr std::string_view kName = "v3_requests_get";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  RequestsGet(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
