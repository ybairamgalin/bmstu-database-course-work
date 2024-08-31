#pragma once

#include "middleware/auth.hpp"

#include <userver/server/middlewares/configuration.hpp>

namespace middleware {

class AuthMiddlewarePipelineBuilder
    : public userver::server::middlewares::HandlerPipelineBuilder {
 public:
  using HandlerPipelineBuilder::HandlerPipelineBuilder;

  userver::server::middlewares::MiddlewaresList BuildPipeline(
      userver::server::middlewares::MiddlewaresList server_middleware_pipeline)
      const override;
};

}  // namespace middleware