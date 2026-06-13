#ifndef REVERSE_FILTER_H
#define REVERSE_FILTER_H

#include "ifilter.h"

class ReverseFilter final : public IFilter {
public:
  void apply(Waveform &waveform) const override;
  
  const char *name() const noexcept override { return "reverse"; }
};

#endif