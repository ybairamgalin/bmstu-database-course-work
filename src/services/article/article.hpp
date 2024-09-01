#pragma once

#include <memory>

#include "repository/factory.hpp"
#include "services/article_service.hpp"

namespace services {

class ArticleService : public IArticleService {
 public:
  explicit ArticleService(const std::shared_ptr<repository::IRepositoryFactory>&
                              repository_factory);
  boost::uuids::uuid CreateArticle(const services::Article& article) override;
  void UpdateArticle(const boost::uuids::uuid& id,
                     const services::Article& article) override;
  ArticleFullInfo GetArticle(const boost::uuids::uuid& id) override;

 private:
  std::unique_ptr<repository::ArticleRepository> article_repository_;
  std::unique_ptr<repository::EventRepository> event_repository_;
  std::unique_ptr<repository::UserDataRepository> user_data_repository_;
};

}  // namespace services
