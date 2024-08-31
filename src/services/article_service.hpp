#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace services {

struct Article {
  std::string title;
  std::string content;
  int64_t author_id;
  boost::uuids::uuid event_id;
};

class IArticleService {
 public:
  virtual ~IArticleService() = default;
  virtual boost::uuids::uuid CreateArticle(const Article& article) = 0;
};

}  // namespace services
