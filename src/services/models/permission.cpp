#include "permission.hpp"

namespace services {

std::string ToString(Permission p) {
  switch (p) {
    case Permission::kModerateRequests:
      return "moderator";
    case Permission::kManageModerators:
      return "admin";
  }

  throw std::runtime_error("Unknown permission");
}

}  // namespace