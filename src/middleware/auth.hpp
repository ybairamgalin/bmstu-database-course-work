#pragma once

#include <memory>

#include <userver/server/middlewares/http_middleware_base.hpp>

#include "handlers/base_handler.hpp"
#include "services/auth_service.hpp"
#include "services/factory.hpp"

namespace middleware {

class Auth : public userver::server::middlewares::HttpMiddlewareBase {
 public:
  static constexpr std::string_view kName = "auth-middleware";

  explicit Auth(std::unique_ptr<services::IAuthService> auth_service);

 private:
  void HandleRequest(
      userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& ctx) const override;

 private:
  std::unique_ptr<services::IAuthService> auth_service_;
};

class AuthFactory
    : public userver::server::middlewares::HttpMiddlewareFactoryBase {
 public:
  static constexpr std::string_view kName = "auth-middleware";

  AuthFactory(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  [[nodiscard]] std::unique_ptr<
      userver::server::middlewares::HttpMiddlewareBase>
  Create(const userver::server::handlers::HttpHandlerBase& handler,
         userver::yaml_config::YamlConfig) const override;

 private:
  std::unique_ptr<services::IServiceFactory> services_;
};

}  // namespace middleware
