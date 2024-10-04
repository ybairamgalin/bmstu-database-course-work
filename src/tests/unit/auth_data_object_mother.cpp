#include "auth_data_object_mother.hpp"
#include "repository/user_data_repository.hpp"

namespace repository {

repository::AuthData AuthDataMother::NewUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "user";
  return auth_data;
}

repository::AuthData AuthDataMother::NewAdminUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "admin";
  return auth_data;
}

repository::AuthData AuthDataMother::NewModeratorUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "moderator";
  return auth_data;
}

}  // namespace repository
