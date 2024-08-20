#include "auth.hpp"

#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>

#include "di/init.hpp"
#include "http/exception.hpp"
#include "services/factory.hpp"
#include "utils/auth.hpp"
#include "utils/json.hpp"
#include "utils/request.hpp"

namespace middleware {

Auth::Auth(std::unique_ptr<services::IAuthService> auth_service)
    : auth_service_(std::move(auth_service)) {}

void Auth::HandleRequest(userver::server::http::HttpRequest& request,
                         userver::server::request::RequestContext& ctx) const {
  try {
    auto auth_data =
        utils::AuthOrThrow(utils::ConvertHeadersToMap(request), auth_service_);
    ctx.SetData("auth", auth_data);
    Next(request, ctx);
  } catch (const http::HttpException& ex) {
    request.SetResponseStatus(userver::server::http::HttpStatus(ex.Get().code));
    auto& response = request.GetHttpResponse();
    response.SetContentType(userver::http::content_type::kApplicationJson);
    response.SetData(utils::ToJsonString(ex.Get()));
  }
}

AuthFactory::AuthFactory(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : userver::server::middlewares::HttpMiddlewareFactoryBase(config, context),
      services_(di::InitFactories(config, context)) {}

std::unique_ptr<userver::server::middlewares::HttpMiddlewareBase>
AuthFactory::Create(const userver::server::handlers::HttpHandlerBase&,
                    userver::yaml_config::YamlConfig) const {
  return std::make_unique<Auth>(services_->MakeAuthService());
}
}  // namespace middleware
