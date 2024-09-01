#include "article_get.hpp"

#include "http/query.hpp"

namespace handlers::v2 {

ArticleGet::ArticleGet(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : BaseJsonHandler(config, context) {}

ArticleGet::Response ArticleGet::HandleJson(
    ArticleGet::Request&& request,
    userver::server::request::RequestContext& ctx) const {
  auto article_id = http::GetRequiredQueryParamOtThrow<boost::uuids::uuid>(
      request.query_params, "article_id");
  auto article = services_->MakeArticleService()->GetArticle(article_id);
  return ArticleGet::Response{
      gen::ArticleGetResponse200{
          article.title, article.content,
          gen::Event{article.event.name, article.event.description},
          gen::UserFullInfo{
              article.author.id,
              article.author.name,
              article.author.login,
              article.author.phone,
          }},
      200,
      {}};
}

}  // namespace handlers::v2
