#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>

#include "services/factory.hpp"

namespace handlers {

class BaseHandler : public userver::server::handlers::HttpHandlerBase {
 public:
  BaseHandler(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  virtual std::string Handle(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& ctx) const = 0;

 private:
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& ctx) const final;

 protected:
  std::shared_ptr<services::IServiceFactory> services_;
};

}  // namespace handlers
