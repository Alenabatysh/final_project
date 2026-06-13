#ifndef SILENCE_FILTER_H
#define SILENCE_FILTER_H

#include "ifilter.h"

class SilenceFilter final : public IFilter {
public:
  SilenceFilter(double start_seconds, double end_seconds);

  void apply(Waveform &wav) const override;
  
  const char *name() const noexcept override { return "silence"; }

private:
  double start_seconds_;
  double end_seconds_;
};

#endif