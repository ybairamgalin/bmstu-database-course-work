#include "auth_pipeline_builder.hpp"

#include "middleware/auth.hpp"

namespace middleware {

userver::server::middlewares::MiddlewaresList
AuthMiddlewarePipelineBuilder::BuildPipeline(
    userver::server::middlewares::MiddlewaresList server_middleware_pipeline)
    const {
  server_middleware_pipeline.emplace_back(AuthFactory::kName);
  return HandlerPipelineBuilder::BuildPipeline(server_middleware_pipeline);
}

}  // namespace middleware
