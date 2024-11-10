#pragma once

#include "repository/user_data_repository.hpp"

namespace repository {

class AuthDataMother {
 public:
  repository::AuthData NewUser();
  repository::AuthData NewAdminUser();
  repository::AuthData NewModeratorUser();
};

}  // namespace repository