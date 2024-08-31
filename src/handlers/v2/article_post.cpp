#include "article_post.hpp"

namespace handlers::v2 {

ArticlePost::ArticlePost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

ArticlePost::Response ArticlePost::HandleJson(
    handlers::BaseJsonHandler<gen::ArticlePostBody,
                              gen::ArticlePostResponse201>::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");

  const auto id = services_->MakeArticleService()->CreateArticle(
      services::Article{request.body.title, request.body.content, auth.user_id,
                        request.body.event_id});
  return Response{gen::ArticlePostResponse201{id}, 201, {}};
}

}  // namespace handlers::v2
