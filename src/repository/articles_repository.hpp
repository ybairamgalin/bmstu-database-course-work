#pragma once

namespace repository {

struct DbArticle {
  boost::uuids::uuid article_id;
  std::string title;
  std::string content;
  int64_t author_id;
  boost::uuids::uuid event_id;
};

class ArticleRepository {
 public:
  virtual ~ArticleRepository() = default;
  virtual void AddArticle(const DbArticle& article) = 0;

};

}