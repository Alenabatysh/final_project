#ifndef TIMESTRETCH_FILTER_H
#define TIMESTRETCH_FILTER_H

#include "ifilter.h"

class TimestretchFilter final : public IFilter {
public:
  explicit TimestretchFilter(double factor);

  void apply(Waveform &wav) const override;
  
  const char *name() const noexcept override { return "timestretch"; }

private:
  double factor_;
};

#endif