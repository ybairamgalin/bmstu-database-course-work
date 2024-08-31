#include "article.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "services/exception.hpp"

namespace services {

ArticleService::ArticleService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : article_repository_(repository_factory->MakeArticleRepository()),
      event_repository_(repository_factory->MakeEventsRepository()) {}

boost::uuids::uuid ArticleService::CreateArticle(const Article& article) {
  if (event_repository_->GetEventsByIds({article.event_id}).empty()) {
    throw ServiceLevelException("Event not found");
  }

  const auto article_id = boost::uuids::random_generator()();
  article_repository_->UpsertArticle(
      repository::DbArticle{article_id, article.title, article.content,
                            article.author_id, article.event_id});
  return article_id;
}

void ArticleService::UpdateArticle(const boost::uuids::uuid& id,
                                   const Article& article) {
  auto storage_article = article_repository_->GetArticle(id);
  if (!storage_article.has_value()) {
    throw ServiceLevelException("Article does not exist");
  }
  if (storage_article.value().author_id != article.author_id) {
    throw ServiceLevelException("Unable to change article of other user",
                                ErrorType::kInvalidInput);
  }
  article_repository_->UpsertArticle(repository::DbArticle{
      id, article.title, article.content, article.author_id, article.event_id});
}

}  // namespace services
