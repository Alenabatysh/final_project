#ifndef NORMALIZE_FILTER_H
#define NORMALIZE_FILTER_H

#include "ifilter.h"

class NormalizeFilter final : public IFilter {
public:
  explicit NormalizeFilter(double peak = 1.0);

  void apply(Waveform &waveform) const override;
  
  const char *name() const noexcept override { return "normalize"; }

private:
  double peak_;
};

#endif