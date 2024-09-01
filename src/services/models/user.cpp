#include "user.hpp"

namespace services {

services::UserInfo MapUser(const repository::AuthData& user) {
  return services::UserInfo{user.user_id, user.name, user.login, user.phone};
}

}  // namespace services