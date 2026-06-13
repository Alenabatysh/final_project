#ifndef PIPELINE_H
#define PIPELINE_H

#include "ifilter.h"
#include <memory>
#include <vector>

class Pipeline {
public:
  void add(std::unique_ptr<IFilter> filter);

  void apply(Waveform &wav) const;

  void clear();

  bool empty() const;

  std::size_t size() const;
  
  void reserve(std::size_t capacity);

private:
  std::vector<std::unique_ptr<IFilter>> filters_;
};

#endif