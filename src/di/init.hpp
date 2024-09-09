#pragma once

#include <memory>

#include <userver/components/component.hpp>
#include <userver/components/component_base.hpp>

#include "services/factory.hpp"

namespace di {

class DiComponent : public userver::components::ComponentBase {
 public:
  static constexpr std::string_view kName = "di-component";

  DiComponent(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);
  ~DiComponent();

  std::unique_ptr<services::IServiceFactory> MakeServiceFactory();

  [[maybe_unused]] static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;

};

}  // namespace di
