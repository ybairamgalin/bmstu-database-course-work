#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/format.h>
#include <boost/uuid/uuid.hpp>

#include <userver/utils/datetime.hpp>
#include <userver/utils/datetime/timepoint_tz.hpp>

#include "../utils/meta.hpp"
#include "exception.hpp"

namespace http {

template <typename ParamType>
ParamType GetRequiredQueryParamOtThrow(
    const std::unordered_map<std::string, std::string>&, const std::string&) {
  static_assert(utils::always_false<ParamType>, "Unknown type");
}

template <typename ParamType>
std::optional<ParamType> GetQueryParamOpt(
    const std::unordered_map<std::string, std::string>&, const std::string&) {
  static_assert(utils::always_false<ParamType>, "Unknown type");
}

template <>
std::string GetRequiredQueryParamOtThrow<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

template <>
std::optional<std::string> GetQueryParamOpt<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

template <>
boost::uuids::uuid GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

template <>
std::optional<boost::uuids::uuid> GetQueryParamOpt<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

template <>
std::optional<userver::utils::datetime::TimePointTz>
GetQueryParamOpt<userver::utils::datetime::TimePointTz>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

}  // namespace http
