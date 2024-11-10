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
  static std::string Run(
      const userver::storages::postgres::ClusterPtr& cluster_ptr);

  static void AddSuite(const std::string& name, const std::vector<Test>& tests);
};

#ifndef DECL_TEST
#define DECL_TEST(t) Test{#t, t}
#endif
