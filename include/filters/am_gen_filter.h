#ifndef AM_GEN_FILTER_H
#define AM_GEN_FILTER_H

#include "ifilter.h"

class AmGenFilter final : public IFilter {
public:
  AmGenFilter(double amplitude, double carrier_hz, double modulation_hz,
              double depth, double duration_ms);
  void apply(Waveform &waveform) const override;
  const char *name() const noexcept override { return "gen_am"; }

private:
  double amplitude_;
  double carrier_hz_;
  double modulation_hz_;
  double depth_;
  double duration_ms_;
};

#endif