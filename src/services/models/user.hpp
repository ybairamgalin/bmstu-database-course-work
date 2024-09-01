#pragma once

#include <string>

#include "repository/user_data_repository.hpp"

namespace services {

struct UserInfo {
  int64_t id;
  std::string name;
  std::string login;
  std::string phone;
};

services::UserInfo MapUser(const repository::AuthData& user);

}  // namespace services
