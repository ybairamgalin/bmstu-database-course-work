#include <userver/utils/daemon_run.hpp>

#include <userver/clients/dns/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include "tests/nonmock/test_component.hpp"

int main(int argc, char* argv[]) {
  return userver::utils::DaemonMain(
      argc, argv,
      userver::components::MinimalServerComponentList()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::clients::dns::Component>()
          .Append<IntegrationTestingComponent>()
          .Append<userver::components::Postgres>("postgres-db-1")
          .Append<userver::components::Mongo>("mongo-db"));
}
