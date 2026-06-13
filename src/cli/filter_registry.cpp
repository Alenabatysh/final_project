#include "cli/filter_registry.h"
#include <stdexcept>

void FilterRegistry::register_filter(const std::string &name,
                                     FilterProducer producer) {
  if (name.empty()) {
    throw std::invalid_argument("Filter name cannot be empty");
  }
  if (!producer) {
    throw std::invalid_argument("Filter producer cannot be empty");
  }
  producers_[name] = std::move(producer);
}

std::unique_ptr<IFilter>
FilterRegistry::create(const FilterDescriptor &descriptor) const {
  auto it = producers_.find(descriptor.name);
  if (it == producers_.end()) {
    throw std::runtime_error("Unknown filter: " + descriptor.name);
  }
  return it->second(descriptor);
}

bool FilterRegistry::has_filter(const std::string &name) const {
  return producers_.find(name) != producers_.end();
}