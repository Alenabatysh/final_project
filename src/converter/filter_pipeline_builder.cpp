#include "converter/filter_pipeline_builder.h"
#include <stdexcept>

FilterPipelineBuilder::FilterPipelineBuilder(const FilterRegistry &registry)
    : registry_(registry) {}

Pipeline FilterPipelineBuilder::build_pipeline(
    const std::vector<FilterDescriptor> &descriptors) const {
  Pipeline pipeline;
  for (const auto &descriptor : descriptors) {
    pipeline.add(registry_.create(descriptor));
  }
  return pipeline;
}