#include "factory.hpp"

#include "auth/yandex_auth.hpp"
#include "event/event.hpp"
#include "idm/idm.hpp"
#include "request_management/request_management.hpp"

namespace services {

SimpleServiceFactory::SimpleServiceFactory(
    std::unique_ptr<repository::IRepositoryFactory> repository_factory)
    : repository_factory_(std::move(repository_factory)) {}

std::unique_ptr<IAuthService> SimpleServiceFactory::MakeAuthService() {
  return std::make_unique<YandexAuthService>(repository_factory_);
}

std::unique_ptr<IIdmService> SimpleServiceFactory::MakeIdmService() {
  return std::make_unique<IdmService>(repository_factory_);
}

std::unique_ptr<IRequestManagementService>
SimpleServiceFactory::MakeRequestManagementService() {
  return std::make_unique<RequestManagementService>(repository_factory_);
}

std::unique_ptr<IEventService> SimpleServiceFactory::MakeEventService() {
  return std::make_unique<EventService>(repository_factory_);
}

}  // namespace services
