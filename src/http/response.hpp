#pragma once

#include <string>
#include <unordered_map>

namespace http {

template <typename Body>
struct ResponseWithBody {
  Body body{};
  int32_t status_code{};
  std::unordered_map<std::string, std::string> headers{};
};

struct ResponseWithoutBody {
  int32_t status_code{};
  std::unordered_map<std::string, std::string> headers{};
};

struct EmptyResponseBody {};

template <typename Body>
struct MetaGetResponseType {
  using ReponseType = ResponseWithBody<Body>;
};

template <>
struct MetaGetResponseType<EmptyResponseBody> {
  using ReponseType = ResponseWithoutBody;
};

template <typename Body>
using Response = typename MetaGetResponseType<Body>::ReponseType;

}  // namespace http
