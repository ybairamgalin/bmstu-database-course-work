#pragma once

#include <optional>
#include <string>

#include <userver/clients/http/client.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "../auth_service.hpp"

namespace services {

class YandexAuthService : public services::IAuthService {
 public:
  YandexAuthService(userver::clients::http::Client& http_client,
                    userver::storages::postgres::ClusterPtr cluster_ptr);
  std::optional<services::AuthData> GetAuthDataByToken(
      const std::string& token) override;

 private:
  userver::clients::http::Client& http_client_;
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace services
