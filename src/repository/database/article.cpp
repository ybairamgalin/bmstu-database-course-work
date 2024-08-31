#include "article.hpp"

#include <utility>

namespace repository {

DbArticleRepository::DbArticleRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void DbArticleRepository::UpsertArticle(const DbArticle& article) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.articles "
                        "(article_id, title, content, author_id, event_id) "
                        "values ($1, $2, $3, $4, $5) "
                        "on conflict (article_id) "
                        "do update set "
                        "    title = $2,"
                        "    content = $3,"
                        "    event_id = $5",
                        article.article_id, article.title, article.content,
                        article.author_id, article.event_id);
}

std::optional<DbArticleFull> DbArticleRepository::GetArticle(
    const boost::uuids::uuid& id) {
  auto result = cluster_ptr_->Execute(
      userver::storages::postgres::ClusterHostType::kSlave,
      "select article_id, title, content, author_id, "
      "event_id, created_at "
      "from service.articles "
      "where article_id = $1",
      id);

  if (result.IsEmpty()) {
    return std::nullopt;
  }
  return result.AsSingleRow<DbArticleFull>(
      userver::storages::postgres::kRowTag);
}

}  // namespace repository
