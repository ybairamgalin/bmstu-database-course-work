#pragma once

#include <boost/uuid/uuid.hpp>

#include <userver/storages/postgres/io/chrono.hpp>

namespace repository {

struct DbArticle {
  boost::uuids::uuid article_id{};
  std::string title{};
  std::string content{};
  int64_t author_id{};
  boost::uuids::uuid event_id{};
};

struct DbArticleFull {
  boost::uuids::uuid article_id{};
  std::string title{};
  std::string content{};
  int64_t author_id{};
  boost::uuids::uuid event_id{};
  userver::storages::postgres::TimePointTz created_at;
};

class ArticleRepository {
 public:
  virtual ~ArticleRepository() = default;
  virtual void UpsertArticle(const DbArticle& article) = 0;
  virtual std::optional<DbArticleFull> GetArticle(
      const boost::uuids::uuid& id) = 0;
};

}  // namespace repository