#include "runner.hpp"

#include <algorithm>
#include <random>

namespace {

std::unordered_map<std::string, std::vector<Test>> suite_to_tests;

void Cleanup(const userver::storages::postgres::ClusterPtr& cluster_ptr) {
  cluster_ptr->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       "delete from service.requests");
  cluster_ptr->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       "delete from service.events");
  cluster_ptr->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       "delete from service.users");
}

}  // namespace

std::string TestRunner::Run(
    const userver::storages::postgres::ClusterPtr& cluster_ptr) {
  std::srand(std::time(nullptr));

  auto repository_factory =
      std::make_shared<repository::PgRepositoryFactory>(cluster_ptr);
  size_t passed = 0;
  size_t total_tests = 0;

  std::string run_log{"fake log"};
  return run_log;
}

void TestRunner::AddSuite(const std::string& name,
                          const std::vector<Test>& tests) {
  suite_to_tests[name] = tests;
}
