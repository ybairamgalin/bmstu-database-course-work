#pragma once

#include <unordered_map>
#include <string>

namespace http {

template <typename Body>
struct RequestWithBody {
  Body body;
  std::vector<std::string> path_args;
  std::unordered_map<std::string, std::string> query_params;
  std::unordered_map<std::string, std::string> headers;
};

struct RequestWithoutBody {
  std::vector<std::string> path_args;
  std::unordered_map<std::string, std::string> query_params;
  std::unordered_map<std::string, std::string> headers;
};

struct EmptyRequestBody {};

template <typename Body>
struct MetaGetRequestType {
  using RequestType = RequestWithBody<Body>;
};

template <>
struct MetaGetRequestType<EmptyRequestBody> {
  using RequestType = RequestWithoutBody;
};

template <typename Body>
using Request = typename MetaGetRequestType<Body>::RequestType;

}
