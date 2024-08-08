#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "gen/common.hpp"
#include "http/exception.hpp"
#include "services/auth_service.hpp"

namespace utils {

services::AuthData AuthOrThrow(
    const std::unordered_map<std::string, std::string>& headers,
    std::unique_ptr<services::IAuthService> auth_service);

}  // namespace utils
