#pragma once

#include <memory>

#include "../repository/factory.hpp"
#include "../repository/requests_repository.hpp"
#include "../repository/user_data_repository.hpp"

class MockUserDataRepository : public repository::UserDataRepository {
 public:
  void SetResponse(const std::optional<repository::AuthData>& response);

  std::optional<repository::AuthData> GetUserData(
      const std::string& token) override;
  std::optional<repository::AuthData> GetUserDataByLogin(
      const std::string& login) override;
  void SaveUserData(const repository::AuthData&) override;

 private:
  std::optional<repository::AuthData> response_ = std::nullopt;
};

class MockRepositoryFactory : public repository::IRepositoryFactory {
 public:
  std::unique_ptr<repository::RequestsRepository> MakeRequestsRepository()
      override;

  void SetUserDataDbRepository(
      std::unique_ptr<repository::UserDataRepository> user_data_repository);
  std::unique_ptr<repository::UserDataRepository> MakeUserDataDbRepository()
      override;

  void SetUserDataHttpRepository(
      std::unique_ptr<repository::UserDataRepository> user_data_repository);
  std::unique_ptr<repository::UserDataRepository> MakeUserDataHttpRepository()
      override;

  std::unique_ptr<repository::FileStorageRepository> MakeFileStorageRepository()
      override;

  std::unique_ptr<repository::FileMetaRepository> MakeFileMetaRepository()
      override;

 private:
  std::unique_ptr<repository::UserDataRepository> db_user_data_ =
      std::make_unique<MockUserDataRepository>();
  std::unique_ptr<repository::UserDataRepository> http_user_data_ =
      std::make_unique<MockUserDataRepository>();
};
