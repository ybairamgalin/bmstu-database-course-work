#pragma once

#include "gen/handlers/request_get.hpp"
#include "handler.hpp"
#include "http/request.hpp"

namespace handlers::v2 {

class RequestGet
    : public BaseHandler<http::EmptyRequestBody, gen::RequestGetResponse200> {
 public:
  static constexpr std::string_view kName = "v2_request_get";

  using BaseHandler<http::EmptyRequestBody,
                    gen::RequestGetResponse200>::Request;
  using BaseHandler<http::EmptyRequestBody,
                    gen::RequestGetResponse200>::Response;

  RequestGet(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
