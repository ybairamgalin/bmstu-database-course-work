#include "runner.hpp"

#include <algorithm>
#include <chrono>
#include <random>

#include <userver/engine/task/task_with_result.hpp>
#include <userver/utils/async.hpp>
#include <userver/logging/log.hpp>

namespace {

std::unordered_map<std::string, std::vector<Test>> suite_to_tests;

void Cleanup(const userver::storages::postgres::ClusterPtr& cluster_ptr) {
  LOG_WARNING() << "Performing cleanup";
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
  Cleanup(cluster_ptr);

  passed_tests_ = 0;
  total_tests_ = 0;

  std::string run_log{};

  std::vector<userver::engine::TaskWithResult<std::string>> tasks;
  for (auto& [suite, tests] : suite_to_tests) {
    run_log += fmt::format("RUNNING SUITE {}\n", suite);
    //    std::random_shuffle(tests.begin(), tests.end());

    auto task = userver::utils::Async(fmt::format("task_for_suite_{}", suite),
                                      &TestRunner::RunSuite, this, cluster_ptr,
                                      std::move(tests));
    tasks.emplace_back(std::move(task));
  }

  for (auto& task : tasks) {
    run_log += task.Get();
  }

  run_log +=
      fmt::format("\nRESULT: PASSED {}/{}\n\n", passed_tests_, total_tests_);
  return run_log;
}

void TestRunner::AddSuite(const std::string& name,
                          const std::vector<Test>& tests) {
  suite_to_tests[name] = tests;
}

std::string TestRunner::RunSuite(
    const userver::storages::postgres::ClusterPtr& cluster_ptr,
    const std::vector<Test>& tests) {
  auto repository_factory =
      std::make_shared<repository::PgRepositoryFactory>(cluster_ptr);

  std::string run_log;
  for (const auto& test : tests) {
    ++total_tests_;
    try {
      test.func(repository_factory);
      ++passed_tests_;
      run_log += fmt::format("{} PASSED\n", test.name);
    } catch (const std::exception& ex) {
      run_log += fmt::format("{} FAILED: {}\n", test.name, ex.what());
    } catch (...) {
      run_log += fmt::format("{} FAILED: unknown exception\n", test.name);
    }
  }

  return run_log;
}

TestRunner::TestRunner() : total_tests_(0), passed_tests_(0) {}
