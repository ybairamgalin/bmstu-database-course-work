#include "request.hpp"

namespace utils {

std::unordered_map<std::string, std::string> ConvertHeadersToMap(
    const userver::server::http::HttpRequest& request) {
  std::unordered_map<std::string, std::string> result;
  result.reserve(request.GetHeaders().size());
  for (const auto& header : request.GetHeaderNames()) {
    result.try_emplace(header, request.GetHeader(header));
  }
  return result;
}

std::unordered_map<std::string, std::string> ConvertQueryParamsToMap(
    const userver::server::http::HttpRequest& request) {
  std::unordered_map<std::string, std::string> result;
  result.reserve(request.ArgNames().size());
  for (const auto& arg : request.ArgNames()) {
    result.try_emplace(arg, request.GetArg(arg));
  }
  return result;
}

}  // namespace utils
