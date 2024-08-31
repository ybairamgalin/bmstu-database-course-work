#include "article.hpp"

#include <utility>

namespace repository {

DbArticleRepository::DbArticleRepository(
    userver::storages::postgres::ClusterPtr cluster_ptr)
    : cluster_ptr_(std::move(cluster_ptr)) {}

void DbArticleRepository::AddArticle(const DbArticle& article) {
  cluster_ptr_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                        "insert into service.articles "
                        "(article_id, title, content, author_id, event_id) "
                        "values ($1, $2, $3, $4, $5)",
                        article.article_id, article.title, article.content,
                        article.author_id, article.event_id);
}

}  // namespace repository
