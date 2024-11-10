#pragma once

#include "handlers/base_handler.hpp"

namespace handlers::v3 {

class FilesPut : public BaseHandler {
 public:
  static constexpr std::string_view kName = "v3_files_put";

  FilesPut(const userver::components::ComponentConfig& config,
          const userver::components::ComponentContext& context);

  std::string Handle(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v3
