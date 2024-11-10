#include "object_mother.hpp"

namespace repository {

repository::AuthData AuthDataMother::NewUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "user";
  auth_data.token = "user_token";
  return auth_data;
}

repository::AuthData AuthDataMother::NewAdminUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "admin";
  auth_data.token = "user_token";
  return auth_data;
}

repository::AuthData AuthDataMother::NewModeratorUser() {
  repository::AuthData auth_data{};
  auth_data.user_id = 1;
  auth_data.login = "login";
  auth_data.name = "Yaroslav";
  auth_data.phone = "+11111";
  auth_data.role = "moderator";
  auth_data.token = "user_token";
  return auth_data;
}

}  // namespace repository