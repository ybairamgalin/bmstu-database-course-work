#include "auth.hpp"

namespace utils {

services::AuthData AuthOrThrow(
    const std::unordered_map<std::string, std::string>& headers,
    const std::unique_ptr<services::IAuthService>& auth_service) {
  const auto it = headers.find("X-Token");
  if (it == headers.end()) {
    throw http::HttpException{401, "Missing auth token"};
  }
  auto auth_data = auth_service->GetAuthDataByToken(it->second);
  if (!auth_data.has_value()) {
    throw http::HttpException{403, "Not enough permissions or user not found"};
  }
  return auth_data.value();
}

}  // namespace utils
