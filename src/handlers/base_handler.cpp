#include "base_handler.hpp"

#include <userver/formats/json.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/server/http/http_status.hpp>

#include "di/init.hpp"
#include "gen/common.hpp"
#include "utils/json.hpp"

namespace {

const gen::ErrorResponse kServerErrorResponse{
    userver::server::http::HttpStatus::kInternalServerError, "Server error"};

}  // namespace

namespace handlers {

BaseHandler::BaseHandler(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerBase(config, context),
      services_(context.FindComponent<di::DiComponent>().MakeServiceFactory()) {
}

std::string BaseHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& ctx) const {
  try {
    return Handle(request, ctx);
  } catch (const std::exception& ex) {
    LOG_ERROR() << fmt::format("Got unhandled exception: {}", ex.what());
  } catch (...) {
    LOG_ERROR() << fmt::format("Got unknown exception");
  }
  request.SetResponseStatus(
      userver::server::http::HttpStatus::kInternalServerError);

  return utils::ToJsonString(kServerErrorResponse);
}

}  // namespace handlers
