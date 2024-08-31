#include "mock_repositories.hpp"

std::unique_ptr<repository::RequestsRepository>
MockRepositoryFactory::MakeRequestsRepository() {
  return nullptr;
}

void MockRepositoryFactory::SetUserDataDbRepository(
    std::unique_ptr<repository::UserDataRepository> user_data_repository) {
  db_user_data_ = std::move(user_data_repository);
}

std::unique_ptr<repository::UserDataRepository>
MockRepositoryFactory::MakeUserDataDbRepository() {
  return std::move(db_user_data_);
}

void MockRepositoryFactory::SetUserDataHttpRepository(
    std::unique_ptr<repository::UserDataRepository> user_data_repository) {
  http_user_data_ = std::move(user_data_repository);
}

std::unique_ptr<repository::UserDataRepository>
MockRepositoryFactory::MakeUserDataHttpRepository() {
  return std::move(http_user_data_);
}

std::unique_ptr<repository::FileStorageRepository>
MockRepositoryFactory::MakeFileStorageRepository() {
  return std::unique_ptr<repository::FileStorageRepository>();
}

std::unique_ptr<repository::FileMetaRepository>
MockRepositoryFactory::MakeFileMetaRepository() {
  return std::unique_ptr<repository::FileMetaRepository>();
}

void MockUserDataRepository::SetResponse(
    const std::optional<repository::AuthData>& response) {
  response_ = response;
}

void MockUserDataRepository::SaveUserData(const repository::AuthData&) {}

std::optional<repository::AuthData> MockUserDataRepository::GetUserData(
    const std::string& token) {
  return response_;
}

std::optional<repository::AuthData> MockUserDataRepository::GetUserDataByLogin(
    const std::string& login) {
  return response_;
}
