#pragma once

#include "../../http/response.hpp"
#include "gen/handlers/manage_access_put.hpp"
#include "handler.hpp"

namespace handlers::v2 {

class ManageAccessPut
    : public BaseHandler<gen::ManageAccessPutBody, http::EmptyResponseBody> {
 public:
  static constexpr std::string_view kName = "v2_manage_access_put";

  using BaseHandler<gen::ManageAccessPutBody, http::EmptyResponseBody>::Request;
  using BaseHandler<gen::ManageAccessPutBody,
                    http::EmptyResponseBody>::Response;

  ManageAccessPut(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

  Response Handle(Request&& request) const override;
};

}  // namespace handlers::v2
