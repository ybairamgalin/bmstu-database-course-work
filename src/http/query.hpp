#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <fmt/format.h>
#include <boost/uuid/uuid.hpp>

#include "../utils/meta.hpp"
#include "exception.hpp"

namespace http {

template <typename ParamType>
ParamType GetRequiredQueryParamOtThrow(
    const std::unordered_map<std::string, std::string>&, const std::string&) {
  static_assert(utils::always_false<ParamType>, "Unknown type");
}

template <>
std::string GetRequiredQueryParamOtThrow<std::string>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

template <>
boost::uuids::uuid GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
    const std::unordered_map<std::string, std::string>& params,
    const std::string& name);

}  // namespace http
