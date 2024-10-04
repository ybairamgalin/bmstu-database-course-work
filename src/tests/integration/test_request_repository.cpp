#include "test_request_repository.hpp"

#include <functional>
#include <iostream>
#include <vector>

#include <fmt/format.h>
#include <boost/uuid/uuid_generators.hpp>

#include <userver/storages/mongo/component.hpp>
#include <userver/utils/assert.hpp>

#include "repository/database/mongo_request.hpp"
#include "repository/database/request.hpp"
#include "repository/requests_repository.hpp"

namespace {

void TestInsert(const std::shared_ptr<repository::RequestsRepository>&
                    requests_repository) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  requests_repository->Insert(request);

  auto repository_request = requests_repository->GetRequestById(request_id);

  UASSERT(repository_request.has_value());
  UASSERT(repository_request.value().request_id == request.request_id);
  UASSERT(repository_request.value().event_id == request.event_id);
  UASSERT(repository_request.value().description == request.description);
  UASSERT(repository_request.value().status == "new");
  UASSERT(repository_request.value().comments.empty());
  UASSERT(repository_request.value().attachments.empty());
}

void TestUpdate(const std::shared_ptr<repository::RequestsRepository>&
                    requests_repository) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  requests_repository->Insert(request);

  request.author_id = 2;
  request.status = "declined";
  request.event_id = boost::uuids::random_generator()();
  request.description = "new_descr";

  requests_repository->Update(request);
  auto repository_request = requests_repository->GetRequestById(request_id);

  UASSERT(repository_request.has_value());
  UASSERT(repository_request.value().request_id == request.request_id);
  UASSERT(repository_request.value().event_id == request.event_id);
  UASSERT(repository_request.value().description == request.description);
  UASSERT(repository_request.value().status == request.status);
  UASSERT(repository_request.value().comments.empty());
  UASSERT(repository_request.value().attachments.empty());
}

void TestAddComment(const std::shared_ptr<repository::RequestsRepository>&
                        requests_repository) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  requests_repository->Insert(request);
  std::string comment_content = "comment content";
  requests_repository->AddComment(request_id, "comment content", 10);

  auto repository_request = requests_repository->GetRequestById(request_id);
  UASSERT(repository_request.has_value());
  UASSERT(repository_request.value().comments.size() == 1);
  UASSERT(repository_request.value().comments.front().author_id == 10);
  UASSERT(repository_request.value().comments.front().content ==
          comment_content);
}

void TestGetAll(const std::shared_ptr<repository::RequestsRepository>&
                    requests_repository) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  requests_repository->Insert(request);
  auto db_requests = requests_repository->GetAll();
  UASSERT(db_requests.size() == 1);
  UASSERT(db_requests.front().request_id == request_id);
  UASSERT(db_requests.front().author_id == 1);
}

}  // namespace

RequestRepositoryIntTestComponent::RequestRepositoryIntTestComponent(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      cluster_ptr_(
          context.FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()),
      pool_(context.FindComponent<userver::components::Mongo>("mongo-db")
                .GetPool()) {
  RunTests();
  std::terminate();
}

struct Test {
  std::string name;
  std::function<void(const std::shared_ptr<repository::RequestsRepository>&)>
      func;
};

#define DECL_TEST(t) \
  Test { #t, t }

void RequestRepositoryIntTestComponent::RunTests() {
  std::vector<Test> tests;
  tests.emplace_back(DECL_TEST(TestInsert));

  tests.emplace_back(DECL_TEST(TestUpdate));
  tests.emplace_back(DECL_TEST(TestAddComment));
  tests.emplace_back(DECL_TEST(TestGetAll));

  auto runner =
      [&](const std::shared_ptr<repository::RequestsRepository>& repository) {
        size_t passed = 0;

        std::string run_log{};
        for (const auto& test : tests) {
          Cleanup();
          try {
            test.func(repository);
            ++passed;
            run_log += fmt::format("{} PASSED\n", test.name);
          } catch (const std::exception& ex) {
            run_log += fmt::format("{} FAILED: {}\n", test.name, ex.what());
          } catch (...) {
            run_log += fmt::format("{} FAILED: unknown exception\n", test.name);
          }
        }

        run_log +=
            fmt::format("\n\nRESULT: PASSED {}/{}\n\n", passed, tests.size());
        return run_log;
      };

  std::string run_log{"Running pg tests\n"};
  run_log +=
      runner(std::make_shared<repository::DbRequestsRepository>(cluster_ptr_));

  std::cerr << run_log;
}

void RequestRepositoryIntTestComponent::Cleanup() {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "delete from service.requests");
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "delete from service.comments");

  auto mongo_requests = pool_->GetCollection("requests");
  mongo_requests.Drop();
}
