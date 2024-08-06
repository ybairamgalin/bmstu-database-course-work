#pragma once

#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json_fwd.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include "../../repository/factory.hpp"
#include "../../services/factory.hpp"

namespace handlers {

class BaseHandlerWithAuth
    : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  BaseHandlerWithAuth(const userver::components::ComponentConfig& config,
                      const userver::components::ComponentContext& context);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext&) const override;

  virtual userver::formats::json::Value Handle(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      const services::AuthData& user_data) const = 0;

 protected:
  std::unique_ptr<services::IServiceFactory> service_factory_;
};

}  // namespace handlers
