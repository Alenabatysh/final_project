#ifndef FILTER_REGISTRY_H
#define FILTER_REGISTRY_H

#include "cli/filter_descriptor.h"
#include "ifilter.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

using FilterProducer =
    std::function<std::unique_ptr<IFilter>(const FilterDescriptor &)>;

class FilterRegistry {
public:
  void register_filter(const std::string &name, FilterProducer producer);

  std::unique_ptr<IFilter> create(const FilterDescriptor &descriptor) const;
  
  bool has_filter(const std::string &name) const;

private:
  std::unordered_map<std::string, FilterProducer> producers_;
};

#endif