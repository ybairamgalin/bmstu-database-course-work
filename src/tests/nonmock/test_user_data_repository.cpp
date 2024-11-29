#include "test_user_data_repository.hpp"

#include "repository/factory.hpp"
#include "repository/user_data_repository.hpp"
#include "runner.hpp"
#include "tests/object_mother.hpp"

void TestSaveUserDataSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data = repository::AuthDataMother().NewUser();
//
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//  user_data_repository->SaveUserData(user_data);
//
//  auto user_opt = user_data_repository->GetUserDataByLogin(user_data.login);
//  UASSERT(user_opt.has_value());
//  UASSERT(user_opt.value().login == user_data.login);
//  UASSERT(user_opt.value().name == user_data.name);
//  UASSERT(user_opt.value().role == user_data.role);
//  UASSERT(user_opt.value().phone == user_data.phone);
}

void TestGetUserDataByIdEmpty(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto users = user_data_repository->GetUserDataByIds({user_data.user_id + 1});
//
//  UASSERT(users.empty());
}

void TestGetUserDataByIdSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto users = user_data_repository->GetUserDataByIds({user_data.user_id});
//
//  UASSERT(users.size() == 1);
//  UASSERT(users.front().login == user_data.login);
}

void TestGetUserDataByLoginEmpty(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto users = user_data_repository->GetUserDataByLogin("bad_login");
//
//  UASSERT(!users.has_value());
}

void TestGetUserDataByLoginSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto user = user_data_repository->GetUserDataByLogin(user_data.login);
//
//  UASSERT(user.has_value());
//  UASSERT(user.value().login == user_data.login);
}

void TestSaveUserRoleSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  user_data_repository->SerUserRole(user_data.login, "admin");
//
//  auto user_opt = user_data_repository->GetUserDataByLogin(user_data.login);
//  UASSERT(user_opt.has_value());
//  UASSERT(user_opt.value().role == "admin");
}

void TestGetUserDataByTokenEmpty(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto user = user_data_repository->GetUserData("bad_token");
//
//  UASSERT(!user.has_value());
}

void TestGetUserDataByTokenSuccess(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory) {
//  auto user_data_repository = repository_factory->MakeUserDataDbRepository();
//
//  auto user_data = repository::AuthDataMother().NewUser();
//  user_data_repository->SaveUserData(user_data);
//
//  auto user = user_data_repository->GetUserData(user_data.token);
//
//  UASSERT(user.has_value());
//  UASSERT(user.value().login == user_data.login);
}

void AddUserDataRepositoryTests() {
  TestRunner::AddSuite("UserDataRepository",
                       {
                           DECL_TEST(TestSaveUserDataSuccess),
                           DECL_TEST(TestGetUserDataByIdEmpty),
                           DECL_TEST(TestGetUserDataByIdSuccess),
                           DECL_TEST(TestSaveUserRoleSuccess),
                           DECL_TEST(TestGetUserDataByLoginEmpty),
                           DECL_TEST(TestGetUserDataByLoginSuccess),
                           DECL_TEST(TestGetUserDataByTokenEmpty),
                           DECL_TEST(TestGetUserDataByTokenSuccess),
                       });
}
