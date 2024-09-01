#include "article.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "services/exception.hpp"

namespace services {

ArticleService::ArticleService(
    const std::shared_ptr<repository::IRepositoryFactory>& repository_factory)
    : article_repository_(repository_factory->MakeArticleRepository()),
      event_repository_(repository_factory->MakeEventsRepository()),
      user_data_repository_(repository_factory->MakeUserDataDbRepository()) {}

boost::uuids::uuid ArticleService::CreateArticle(const Article& article) {
  if (article.author.role == AuthRole::kUser) {
    throw ServiceLevelException("You cannot create articles",
                                ErrorType::kPermissionDenied);
  }

  if (event_repository_->GetEventsByIds({article.event_id}).empty()) {
    throw ServiceLevelException("Event not found");
  }

  const auto article_id = boost::uuids::random_generator()();
  article_repository_->UpsertArticle(
      repository::DbArticle{article_id, article.title, article.content,
                            article.author.user_id, article.event_id});
  return article_id;
}

void ArticleService::UpdateArticle(const boost::uuids::uuid& id,
                                   const Article& article) {
  if (article.author.role == AuthRole::kUser) {
    throw ServiceLevelException("You cannot update articles",
                                ErrorType::kPermissionDenied);
  }

  auto storage_article = article_repository_->GetArticle(id);
  if (!storage_article.has_value()) {
    throw ServiceLevelException("Article does not exist");
  }
  article_repository_->UpsertArticle(
      repository::DbArticle{id, article.title, article.content,
                            article.author.user_id, article.event_id});
}

ArticleFullInfo ArticleService::GetArticle(const boost::uuids::uuid& id) {
  auto db_article_opt = article_repository_->GetArticle(id);
  if (!db_article_opt.has_value()) {
    throw ServiceLevelException("Article not found");
  }
  ArticleFullInfo result{};
  result.title = db_article_opt.value().title;
  result.content = db_article_opt.value().content;

  auto users = user_data_repository_->GetUserDataByIds(
      {db_article_opt.value().author_id});
  if (users.empty()) {
    throw ServiceLevelException("Cannot find user", ErrorType::kInternalError);
  }
  result.author = MapUser(users.front());
  auto events = event_repository_->GetEventsByIds({db_article_opt->event_id});
  if (events.empty()) {
    throw ServiceLevelException("Cannot find event", ErrorType::kInternalError);
  }
  result.event = Event{events.front().name, events.front().description};
  return result;
}

}  // namespace services
