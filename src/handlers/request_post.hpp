#pragma once

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json_fwd.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace handlers {

class RequestPost : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "request_post";

  RequestPost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& component_context);

  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext& context) const override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace handlers
