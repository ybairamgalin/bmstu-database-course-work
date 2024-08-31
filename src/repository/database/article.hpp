#pragma once

#include <boost/uuid/uuid.hpp>

#include <userver/storages/postgres/cluster.hpp>

#include "repository/articles_repository.hpp"

namespace repository {

class DbArticleRepository : public ArticleRepository {
 public:
  explicit DbArticleRepository(
      userver::storages::postgres::ClusterPtr cluster_ptr);

  void AddArticle(const DbArticle& article) override;

 private:
  userver::storages::postgres::ClusterPtr cluster_ptr_;
};

}  // namespace repository
