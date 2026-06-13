#ifndef FILTER_PIPELINE_BUILDER_H
#define FILTER_PIPELINE_BUILDER_H

#include "cli/filter_descriptor.h"
#include "cli/filter_registry.h"
#include "pipeline.h"
#include <vector>

class FilterPipelineBuilder {
public:
  explicit FilterPipelineBuilder(const FilterRegistry &registry);
  Pipeline
  build_pipeline(const std::vector<FilterDescriptor> &descriptors) const;

private:
  const FilterRegistry &registry_;
};

#endif