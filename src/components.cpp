#include "components.hpp"

#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include "handlers/v2/event_post.hpp"
#include "handlers/v2/manage_access_put.hpp"
#include "handlers/v2/request_comment_post.hpp"
#include "handlers/v2/request_get.hpp"
#include "handlers/v2/request_post.hpp"

userver::components::ComponentList MakeComponents() {
  return userver::components::MinimalServerComponentList()
      .Append<userver::server::handlers::Ping>()
      .Append<userver::components::TestsuiteSupport>()
      .Append<userver::components::HttpClient>()
      .Append<userver::server::handlers::TestsControl>()
      .Append<userver::clients::dns::Component>()
      // postgres
      .Append<userver::components::Postgres>("postgres-db-1")
      // handlers
      .Append<handlers::v2::RequestPost>()
      .Append<handlers::v2::RequestGet>()
      .Append<handlers::v2::EventPost>()
      .Append<handlers::v2::RequestCommentPost>()
      .Append<handlers::v2::ManageAccessPut>();
}
