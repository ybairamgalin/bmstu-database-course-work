#pragma once

#include <stdexcept>

namespace services {

enum class Permission {
  kModerateRequests,
  kManageModerators,
};

std::string ToString(Permission p);

}  // namespace services
