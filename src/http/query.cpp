#include "query.hpp"

#include "utils/uuid.hpp"

namespace http {

template <>
std::string GetRequiredQueryParamOtThrow<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  const auto value_opt = GetQueryParamOpt<std::string>(params, name);
  if (!value_opt.has_value()) {
    throw http::HttpException(
        400, fmt::format("Missing required query param {}", name));
  }
  return value_opt.value();
}

template <>
std::optional<std::string> GetQueryParamOpt<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  const auto it = params.find(name);
  if (it == params.end()) {
    return std::nullopt;
  }
  return it->second;
}

template <>
boost::uuids::uuid GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  auto uuid_opt = GetQueryParamOpt<boost::uuids::uuid>(params, name);
  if (!uuid_opt.has_value()) {
    throw http::HttpException(
        400, fmt::format("Missing required query param {}", name));
  }
  return uuid_opt.value();
}

template <>
std::optional<boost::uuids::uuid> GetQueryParamOpt<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name) {
  auto str_uuid_opt = GetQueryParamOpt<std::string>(params, name);
  if (!str_uuid_opt.has_value()) {
    return std::nullopt;
  }
  auto uuid_opt = utils::ToUuid(str_uuid_opt.value());
  if (!uuid_opt.has_value()) {
    throw http::HttpException(
        400,
        fmt::format("String '{}' is not a valid uuid", str_uuid_opt.value()));
  }
  return uuid_opt.value();
}

}  // namespace http
