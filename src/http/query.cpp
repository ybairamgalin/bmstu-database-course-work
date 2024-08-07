#include "query.hpp"

#include "utils/uuid.hpp"

namespace {

std::string GetStringOrThrow(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  const auto it = params.find(name);
  if (it == params.end()) {
    throw http::HttpException(
        400, fmt::format("Missing required query param {}", name));
  }
  return it->second;
}

}  // namespace

namespace http {

template <>
std::string GetRequiredQueryParamOtThrow<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  return GetStringOrThrow(params, name);
}

template <>
boost::uuids::uuid GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  auto str_uuid = GetStringOrThrow(params, name);
  auto uuid_opt = utils::ToUuid(str_uuid);
  if (!uuid_opt.has_value()) {
    throw http::HttpException(
        400, fmt::format("String '{}' is not a valid uuid", str_uuid));
  }
  return uuid_opt.value();
}

}  // namespace http
