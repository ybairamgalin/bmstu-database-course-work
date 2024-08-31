#pragma once

#include "base_handler.hpp"

#include "../http/request.hpp"
#include "../http/response.hpp"

namespace handlers {

template <typename RequestBody, typename ResponseBody>
class BaseJsonHandler : public BaseHandler {
 public:
  using Request = http::Request<RequestBody>;
  using Response = http::Response<ResponseBody>;

  BaseJsonHandler(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

  virtual Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const = 0;

 private:
  std::string Handle(const userver::server::http::HttpRequest& request,
                     userver::server::request::RequestContext& ctx) const final;

  Request ParseRequest(const userver::server::http::HttpRequest& request) const;
  std::string Process(Request&& parsed_request,
                      const userver::server::http::HttpRequest& request,
                      userver::server::request::RequestContext& ctx) const;
};

}  // namespace handlers
