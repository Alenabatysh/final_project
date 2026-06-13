#ifndef AMPL_FILTER_H
#define AMPL_FILTER_H

#include "ifilter.h"

class AmplFilter final : public IFilter {
public:
  explicit AmplFilter(double factor) : factor_(factor) {}

  void apply(Waveform &wav) const override;
  
  const char *name() const noexcept override { return "ampl"; }

private:
  double factor_;
};

#endif