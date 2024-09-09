#include "factory.hpp"

#include "article/article.hpp"
#include "auth/yandex_auth.hpp"
#include "event/event.hpp"
#include "file_service/file_service.hpp"
#include "idm/idm.hpp"
#include "idm/simple_idm_service.hpp"
#include "request_management/request_management.hpp"
#include "services/permission/permission.hpp"

namespace services {

SimpleServiceFactory::SimpleServiceFactory(
    std::unique_ptr<repository::IRepositoryFactory> repository_factory)
    : repository_factory_(std::move(repository_factory)) {}

std::unique_ptr<IAuthService> SimpleServiceFactory::MakeAuthService() {
  return std::make_unique<YandexAuthService>(repository_factory_);
}

std::unique_ptr<IIdmService> SimpleServiceFactory::MakeIdmService() {
  return std::make_unique<SimpleIdmService>(repository_factory_);
}

std::unique_ptr<IRequestManagementService>
SimpleServiceFactory::MakeRequestManagementService() {
  return std::make_unique<RequestManagementService>(repository_factory_);
}

std::unique_ptr<IEventService> SimpleServiceFactory::MakeEventService() {
  return std::make_unique<EventService>(repository_factory_);
}

std::unique_ptr<IFileService> SimpleServiceFactory::MakeFileService() {
  return std::make_unique<FileService>(repository_factory_);
}

std::unique_ptr<IArticleService> SimpleServiceFactory::MakeArticleService() {
  return std::make_unique<ArticleService>(repository_factory_);
}

std::unique_ptr<IPermissionService>
SimpleServiceFactory::MakePermissionService() {
  return std::make_unique<PermissionService>(repository_factory_);
}

}  // namespace services
