#pragma once

#include "request_post_fwd.hpp"

#include <optional>
#include <string>
#include <userver/chaotic/object.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace gen {
namespace impl {}  // namespace impl
}  // namespace gen

namespace gen {

struct HelloRequestBody {
  std::optional<std::string> name{};
};

bool operator==(const gen::HelloRequestBody& lhs,
                const gen::HelloRequestBody& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const gen::HelloRequestBody& value);

HelloRequestBody Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<gen::HelloRequestBody>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const gen::HelloRequestBody& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace gen
