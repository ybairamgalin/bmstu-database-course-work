#include "test_request_repository.hpp"

#include <iostream>

#include <fmt/format.h>
#include <boost/uuid/uuid_generators.hpp>

#include <userver/utils/assert.hpp>

#include "repository/factory.hpp"
#include "repository/requests_repository.hpp"
#include "runner.hpp"

namespace {

void TestInsert(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  auto requests_repository = repository_factory->MakeRequestsRepository();
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

void TestUpdate(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  auto requests_repository = repository_factory->MakeRequestsRepository();
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

void TestAddComment(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  auto requests_repository = repository_factory->MakeRequestsRepository();
  requests_repository->Insert(request);
  std::string comment_content = "comment content";

  requests_repository->AddComment(request_id, comment_content, 10);

  auto repository_request = requests_repository->GetRequestById(request_id);
  UASSERT(repository_request.has_value());
  UASSERT(repository_request.value().comments.size() == 1);
  UASSERT(repository_request.value().comments.front().author_id == 10);
  UASSERT(repository_request.value().comments.front().content ==
comment_content);
}

void TestGetAll(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
  const auto request_id = boost::uuids::random_generator()();

  repository::Request request{};
  request.request_id = request_id;
  request.event_id = boost::uuids::random_generator()();
  request.author_id = 1;
  request.description = "descr";
  request.attachments = {};

  auto requests_repository = repository_factory->MakeRequestsRepository();
  requests_repository->Insert(request);
  auto db_requests = requests_repository->GetAll();
  UASSERT(db_requests.size() >= 1);
}

}  // namespace

void AddRequestRepositoryTests() {
  TestRunner::AddSuite("RequestRepository",
                       {DECL_TEST(TestInsert), DECL_TEST(TestUpdate),
                        DECL_TEST(TestAddComment), DECL_TEST(TestGetAll)});
}
