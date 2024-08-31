#pragma once

#include "gen/handlers/article_post.hpp"
#include "handlers/base_json_handler.hpp"
#include "http/request.hpp"

namespace handlers::v2 {

class ArticlePost : public BaseJsonHandler<gen::ArticlePostBody,
                                           gen::ArticlePostResponse201> {
 public:
  static constexpr std::string_view kName = "v2_article_post";

  using BaseJsonHandler::Request;
  using BaseJsonHandler::Response;

  ArticlePost(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

  Response HandleJson(
      Request&& request,
      userver::server::request::RequestContext& ctx) const override;
};

}  // namespace handlers::v2
