#ifndef SIN_GEN_FILTER_H
#define SIN_GEN_FILTER_H

#include "ifilter.h"

class SinGeneratorFilter final : public IFilter {
public:
  SinGeneratorFilter(double frequency_hz, double duration_ms);

  void apply(Waveform &wav) const override;
  
  const char *name() const noexcept override { return "gen_sin"; }

private:
  double frequency_hz_;
  double duration_ms_;
};

#endif