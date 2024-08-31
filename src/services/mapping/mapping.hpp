#pragma once

#include "../../repository/user_data_repository.hpp"
#include "../models/permission.hpp"

namespace services::internal {

services::AuthData MapAuthData(const repository::AuthData& auth_data);

}  // namespace services::internal
