#pragma once

#include <memory>

#include <userver/components/component.hpp>

#include "services/factory.hpp"

namespace di {

std::unique_ptr<services::IServiceFactory> InitFactories(
    const userver::components::ComponentConfig&,
    const userver::components::ComponentContext& context);

}
