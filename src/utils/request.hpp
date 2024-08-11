#pragma once

#include <string>
#include <unordered_map>

#include <userver/server/http/http_request.hpp>

namespace utils {

std::unordered_map<std::string, std::string> ConvertHeadersToMap(
    const userver::server::http::HttpRequest& request);

std::unordered_map<std::string, std::string> ConvertQueryParamsToMap(
    const userver::server::http::HttpRequest& request);

}  // namespace utils
