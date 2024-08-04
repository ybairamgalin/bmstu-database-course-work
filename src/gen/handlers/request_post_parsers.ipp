#pragma once

#include "request_post.hpp"

namespace gen {

static constexpr USERVER_NAMESPACE::utils::TrivialSet
    kgen__HelloRequestBody_PropertiesNames = [](auto selector) {
      return selector().template Type<std::string_view>().Case("name");
    };

template <typename Value>
gen::HelloRequestBody Parse(
    Value value, USERVER_NAMESPACE::formats::parse::To<gen::HelloRequestBody>) {
  value.CheckNotMissing();
  value.CheckObjectOrNull();

  gen::HelloRequestBody res;

  res.name = value["name"]
                 .template As<std::optional<
                     USERVER_NAMESPACE::chaotic::Primitive<std::string>>>();

  USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
      value, kgen__HelloRequestBody_PropertiesNames);

  return res;
}

}  // namespace gen
