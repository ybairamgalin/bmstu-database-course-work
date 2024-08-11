#pragma once

#include <string>

#include <userver/formats/json.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value.hpp>

namespace utils {

template <typename Type>
std::string ToJsonString(Type value) {
  return userver::formats::json::ToString(Serialize(
      value, userver::formats::serialize::To<userver::formats::json::Value>()));
}

template <typename Type>
userver::formats::json::Value ToJson(Type value) {
  return Serialize(
      value, userver::formats::serialize::To<userver::formats::json::Value>());
}

}  // namespace utils
