#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/json_fwd.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include "gen/common.hpp"
#include "gen/handlers/manage_access_put.hpp"
#include "gen/handlers/request_comment_post.hpp"
#include "gen/handlers/request_post.hpp"

#include "di/init.hpp"
#include "http/exception.hpp"
#include "http/request.hpp"
#include "http/response.hpp"
#include "services/exception.hpp"
#include "services/factory.hpp"

namespace {

template <typename T>
userver::formats::json::Value ToJson(T&& value) {
  return Serialize(
      value, userver::formats::serialize::To<userver::formats::json::Value>());
}

}  // namespace

namespace handlers::v2 {

template <typename RequestBody, typename ResponseBody>
class BaseHandler : public userver::server::handlers::HttpHandlerBase {
 public:
  using Request = http::Request<RequestBody>;
  using Response = http::Response<ResponseBody>;

 public:
  BaseHandler(const userver::components::ComponentConfig&,
              const userver::components::ComponentContext& context);

  virtual Response Handle(Request&& request) const = 0;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const final;

 private:
  std::optional<userver::formats::json::Value> HandleRequestJsonThrowUnsafe(
      const userver::server::http::HttpRequest& request,
      const userver::formats::json::Value& request_json,
      userver::server::request::RequestContext&) const;

 protected:
  std::unique_ptr<services::IServiceFactory> services_;
};

template <typename RequestBody, typename ResponseBody>
BaseHandler<RequestBody, ResponseBody>::BaseHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      services_(di::InitFactories(config, context)) {}

template <typename RequestBody, typename ResponseBody>
std::string BaseHandler<RequestBody, ResponseBody>::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& ctx) const {
  auto& response = request.GetHttpResponse();
  response.SetContentType(userver::http::content_type::kApplicationJson);

  userver::formats::json::Value json;
  if constexpr (!std::is_same_v<RequestBody, http::EmptyRequestBody>) {
    try {
      json = userver::formats::json::FromString(request.RequestBody());
    } catch (const std::exception& ex) {
      return userver::formats::json::ToString(
          ToJson(gen::ErrorResponse{400, ex.what()}));
    }
  }
  try {
    auto result = HandleRequestJsonThrowUnsafe(request, json, ctx);
    if (!result.has_value()) {
      return "";
    }
    return userver::formats::json::ToString(std::move(result.value()));
  } catch (const std::exception& ex) {
    LOG_ERROR() << fmt::format("Got unhandled exception: {}", ex.what());
  } catch (...) {
    LOG_ERROR() << fmt::format("Got unknown exception");
  }
  return userver::formats::json::ToString(
      ToJson(gen::ErrorResponse{500, "Server error"}));
}

template <typename RequestBody, typename ResponseBody>
std::optional<userver::formats::json::Value>
BaseHandler<RequestBody, ResponseBody>::HandleRequestJsonThrowUnsafe(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& request_json,
    userver::server::request::RequestContext&) const {
  Request parsed_request;

  if constexpr (!std::is_same_v<RequestBody, http::EmptyRequestBody>) {
    try {
      parsed_request.body =
          gen::Parse(request_json, userver::formats::parse::To<RequestBody>());
    } catch (const std::exception& ex) {
      LOG_WARNING() << fmt::format("HandleRequestJsonThrow exception: {}",
                                   ex.what());
      return ToJson(gen::ErrorResponse{400, ex.what()});
    }
  }

  parsed_request.query_params.reserve(request.ArgNames().size());
  for (const auto& arg : request.ArgNames()) {
    parsed_request.query_params.try_emplace(arg, request.GetArg(arg));
  }

  parsed_request.headers.reserve(request.GetHeaders().size());
  for (const auto& header : request.GetHeaderNames()) {
    parsed_request.headers.try_emplace(header, request.GetHeader(header));
  }

  decltype(Handle(std::move(parsed_request))) response;
  try {
    response = Handle(std::move(parsed_request));
    request.SetResponseStatus(
        userver::server::http::HttpStatus(response.status_code));
  } catch (const http::HttpException& ex) {
    request.SetResponseStatus(userver::server::http::HttpStatus(ex.Get().code));
    return ToJson(ex.Get());
  } catch (const services::ServiceLevelException& ex) {
    request.SetResponseStatus(
        userver::server::http::HttpStatus(ex.GetStatusCode()));
    return ToJson(gen::ErrorResponse{ex.GetStatusCode(), ex.what()});
  }

  if constexpr (std::is_same_v<ResponseBody, http::EmptyResponseBody>) {
    return std::nullopt;
  } else {
    return ToJson(std::move(response.body));
  }
}

}  // namespace handlers::v2
