#pragma once

#include <string_view>

#include "../http/exception.hpp"
#include "../services/exception.hpp"
#include "base_json_handler_fwd.hpp"

#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value.hpp>

#include <../utils/json.hpp>
#include <../utils/request.hpp>
#include <gen/common.hpp>

namespace gen {

template <typename ToType>
ToType Parse(userver::formats::json::Value,
             userver::formats::parse::To<ToType>);

}

namespace handlers {

template <typename RequestBody, typename ResponseBody>
BaseJsonHandler<RequestBody, ResponseBody>::BaseJsonHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : BaseHandler(config, context) {}

template <typename RequestBody, typename ResponseBody>
std::string BaseJsonHandler<RequestBody, ResponseBody>::Handle(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& ctx) const {
  auto& response = request.GetHttpResponse();
  response.SetContentType(userver::http::content_type::kApplicationJson);

  Request parsed_request;
  try {
    parsed_request = ParseRequest(request);
  } catch (const std::exception& ex) {
    LOG_WARNING() << fmt::format("Parse exception: {}", ex.what());
    request.SetResponseStatus(userver::server::http::HttpStatus(400));
    return utils::ToJsonString(gen::ErrorResponse{400, ex.what()});
  }
  return Process(std::move(parsed_request), request, ctx);
}

template <typename RequestBody, typename ResponseBody>
typename BaseJsonHandler<RequestBody, ResponseBody>::Request
BaseJsonHandler<RequestBody, ResponseBody>::ParseRequest(
    const userver::server::http::HttpRequest& request) const {
  Request parsed_request;
  if constexpr (!std::is_same_v<RequestBody, http::EmptyRequestBody>) {
    auto json = userver::formats::json::FromString(request.RequestBody());
    parsed_request.body =
        gen::Parse(json, userver::formats::parse::To<RequestBody>());
  }
  parsed_request.path_args = utils::GetPathParamsVector(request);
  parsed_request.query_params = utils::ConvertQueryParamsToMap(request);
  parsed_request.headers = utils::ConvertHeadersToMap(request);
  return parsed_request;
}

template <typename RequestBody, typename ResponseBody>
std::string BaseJsonHandler<RequestBody, ResponseBody>::Process(
    Request&& parsed_request, const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& ctx) const {
  Response response{};
  try {
    response = HandleJson(std::move(parsed_request), ctx);
  } catch (const http::HttpException& ex) {
    request.SetResponseStatus(userver::server::http::HttpStatus(ex.Get().code));
    return utils::ToJsonString(ex.Get());
  } catch (const services::ServiceLevelException& ex) {
    request.SetResponseStatus(
        userver::server::http::HttpStatus(ex.GetStatusCode()));
    return utils::ToJsonString(
        gen::ErrorResponse{ex.GetStatusCode(), ex.what()});
  }
  request.SetResponseStatus(
      userver::server::http::HttpStatus(response.status_code));
  if constexpr (!std::is_same_v<ResponseBody, http::EmptyResponseBody>) {
    auto json_response = utils::ToJson(response.body);
    if (json_response.IsEmpty()) {
      return {};
    }
    return userver::formats::json::ToString(json_response);
  }
  return {};
}

}  // namespace handlers
