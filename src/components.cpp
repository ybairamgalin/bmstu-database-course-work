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
#include "handlers/v2/event_list_get.hpp"
#include "handlers/v2/event_post.hpp"
#include "handlers/v2/file_put.hpp"
#include "handlers/v2/manage_access_put.hpp"
#include "handlers/v2/request_comment_post.hpp"
#include "handlers/v2/request_get.hpp"
#include "handlers/v2/request_list_get.hpp"
#include "handlers/v2/request_post.hpp"
#include "handlers/v3/events_delete.hpp"
#include "handlers/v3/events_get.hpp"
#include "handlers/v3/events_id_get.hpp"
#include "handlers/v3/events_id_put.hpp"
#include "handlers/v3/events_post.hpp"
#include "handlers/v3/files_put.hpp"
#include "handlers/v3/requests_get.hpp"
#include "handlers/v3/requests_id_comments.hpp"
#include "handlers/v3/requests_id_get.hpp"
#include "handlers/v3/requests_post.hpp"
#include "handlers/v3/roles.hpp"
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
      // v2
      .Append<handlers::v2::RequestPost>()
      .Append<handlers::v2::RequestGet>()
      .Append<handlers::v2::RequestCommentPost>()
      .Append<handlers::v2::RequestListGet>()
      .Append<handlers::v2::EventPost>()
      .Append<handlers::v2::EventListGet>()
      .Append<handlers::v2::ManageAccessPut>()
      .Append<handlers::v2::FilePut>()
      // v3
      .Append<handlers::v3::RequestsPost>()
      .Append<handlers::v3::EventsPost>()
      .Append<handlers::v3::RequestsIdGet>()
      .Append<handlers::v3::RequestsIdCommentsPost>()
      .Append<handlers::v3::RolesPut>()
      .Append<handlers::v3::RequestsGet>()
      .Append<handlers::v3::EventsGet>()
      .Append<handlers::v3::EventsIdPut>()
      .Append<handlers::v3::FilesPut>()
      .Append<handlers::v3::EventsDelete>()
      .Append<handlers::v3::EventsIdGet>();
}
