#include "article_post.hpp"

#include "http/query.hpp"

namespace handlers::v2 {

ArticlePost::ArticlePost(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

ArticlePost::Response ArticlePost::HandleJson(
    handlers::BaseJsonHandler<gen::ArticlePostBody,
                              gen::ArticlePostResponse201>::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto& auth = ctx.GetData<services::AuthData>("auth");
  auto article_id_opt = http::GetQueryParamOpt<boost::uuids::uuid>(
      request.query_params, "article_id");

  services::Article article{request.body.title, request.body.content, auth,
                            request.body.event_id};
  if (article_id_opt.has_value()) {
    services_->MakeArticleService()->UpdateArticle(article_id_opt.value(),
                                                   article);
    return Response{
        gen::ArticlePostResponse201{article_id_opt.value()}, 200, {}};
  }

  const auto id = services_->MakeArticleService()->CreateArticle(article);
  return Response{gen::ArticlePostResponse201{id}, 201, {}};
}

}  // namespace handlers::v2
