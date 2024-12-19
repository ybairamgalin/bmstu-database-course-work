#pragma once

#include <functional>
#include <memory>
#include <string>

#include "repository/factory.hpp"
#include "repository/requests_repository.hpp"

struct Test {
  std::string name;
  std::function<void(const std::shared_ptr<repository::IRepositoryFactory>&)>
      func;
};

class TestRunner {
 public:
  TestRunner();

  std::string Run(const userver::storages::postgres::ClusterPtr& cluster_ptr);

  static void AddSuite(const std::string& name, const std::vector<Test>& tests);

 private:
  std::string RunSuite(
      const userver::storages::postgres::ClusterPtr& cluster_ptr,
      const std::vector<Test>& tests);

  std::atomic<size_t> total_tests_;
  std::atomic<size_t> passed_tests_;
};

#ifndef DECL_TEST
#define DECL_TEST(t) \
  Test { #t, t }
#endif
