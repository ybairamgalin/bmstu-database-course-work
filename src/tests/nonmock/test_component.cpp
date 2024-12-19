#include "test_component.hpp"

#include <iostream>
#include <signal.h>
#include <userver/utils/periodic_task.hpp>

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
              .GetCluster()),
      syncer_(userver::utils::PeriodicTask{}) {
  LOG_WARNING() << "Starting inttest component";

  {
    AddRequestRepositoryTests();
    AddEventRepositoryTests();
    AddUserDataRepositoryTests();
  }

  std::chrono::seconds interval{2000};
  userver::utils::PeriodicTask::Settings settings{interval};
  settings.flags |= userver::utils::PeriodicTask::Flags::kNow;
  syncer_.Start("run_intttest_task", settings, [this] { RunTests(); });
}

void IntegrationTestingComponent::RunTests() {
  std::string run_log{"Running pg tests\n"};
  run_log += TestRunner().Run(cluster_ptr_);
  std::cerr << run_log;

  std::terminate();
}