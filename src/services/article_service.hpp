#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

#include "models/user.hpp"
#include "services/event_service.hpp"
#include "services/auth_service.hpp"

namespace services {

struct Article {
  std::string title;
  std::string content;
  AuthData author;
  boost::uuids::uuid event_id;
};

struct ArticleFullInfo {
  std::string title;
  std::string content;
  UserInfo author;
  Event event;
};

class IArticleService {
 public:
  virtual ~IArticleService() = default;
  virtual ArticleFullInfo GetArticle(const boost::uuids::uuid& id) = 0;
  virtual boost::uuids::uuid CreateArticle(const Article& article) = 0;
  virtual void UpdateArticle(const boost::uuids::uuid& id,
                             const Article& article) = 0;
};

}  // namespace services
