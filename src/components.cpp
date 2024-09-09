#include "components.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include "di/init.hpp"
#include "handlers/v2/article_get.hpp"
#include "handlers/v2/article_post.hpp"
#include "handlers/v2/event_list_get.hpp"
#include "handlers/v2/event_post.hpp"
#include "handlers/v2/file_put.hpp"
#include "handlers/v2/manage_access_put.hpp"
#include "handlers/v2/request_comment_post.hpp"
#include "handlers/v2/request_get.hpp"
#include "handlers/v2/request_list_get.hpp"
#include "handlers/v2/request_post.hpp"
#include "middleware/auth.hpp"
#include "middleware/auth_pipeline_builder.hpp"

userver::components::ComponentList MakeComponents() {
  return userver::components::MinimalServerComponentList()
      .Append<userver::server::handlers::Ping>()
      .Append<userver::components::TestsuiteSupport>()
      .Append<userver::components::HttpClient>()
      .Append<userver::server::handlers::TestsControl>()
      .Append<userver::clients::dns::Component>()
      // di
      .Append<di::DiComponent>()
      // postgres
      .Append<userver::components::Postgres>("postgres-db-1")
      // mongo
      .Append<userver::components::Mongo>("mongo-db")
      // middleware
      .Append<middleware::AuthFactory>()
      // pipeline builder
      .Append<middleware::AuthMiddlewarePipelineBuilder>(
          "auth-pipeline-builder")
      // handlers
      .Append<handlers::v2::RequestPost>()
      .Append<handlers::v2::RequestGet>()
      .Append<handlers::v2::RequestCommentPost>()
      .Append<handlers::v2::RequestListGet>()
      .Append<handlers::v2::EventPost>()
      .Append<handlers::v2::EventListGet>()
      .Append<handlers::v2::ManageAccessPut>()
      .Append<handlers::v2::FilePut>()
      .Append<handlers::v2::ArticleGet>()
      .Append<handlers::v2::ArticlePost>();
}
