#include "test_component.hpp"

#include <iostream>

#include "runner.hpp"

#include "test_event_repository.hpp"
#include "test_request_repository.hpp"
#include "test_user_data_repository.hpp"

IntegrationTestingComponent::IntegrationTestingComponent(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      cluster_ptr_(
          context.FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()) {
  RunTests();
  exit(0);
}

void IntegrationTestingComponent::RunTests() {
  {
    AddRequestRepositoryTests();
    AddEventRepositoryTests();
    AddUserDataRepositoryTests();
  }

  std::string run_log{"Running pg tests\n"};
  run_log += TestRunner::Run(cluster_ptr_);

  std::cerr << run_log;
}