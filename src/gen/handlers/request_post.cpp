#include "request_post.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include <userver/chaotic/primitive.hpp>
#include <userver/chaotic/validators.hpp>
#include <userver/chaotic/with_type.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "request_post_parsers.ipp"

namespace gen {

bool operator==(const gen::HelloRequestBody& lhs,
                const gen::HelloRequestBody& rhs) {
  return lhs.name == rhs.name && true;
}

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const gen::HelloRequestBody& value) {
  return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value)
                            .ExtractValue());
}

HelloRequestBody Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<gen::HelloRequestBody> to) {
  return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
}

USERVER_NAMESPACE::formats::json::Value Serialize(
    [[maybe_unused]] const gen::HelloRequestBody& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>) {
  USERVER_NAMESPACE::formats::json::ValueBuilder vb =
      USERVER_NAMESPACE::formats::common::Type::kObject;

  if (value.name) {
    vb["name"] =
        USERVER_NAMESPACE::chaotic::Primitive<std::string>{*value.name};
  }

  return vb.ExtractValue();
}

}  // namespace gen
