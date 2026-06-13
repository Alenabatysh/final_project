#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

#include "ifilter.h"
#include <cstddef>

class LowpassFilter final : public IFilter {
public:
  explicit LowpassFilter(std::size_t window_size);

  void apply(Waveform &waveform) const override;
  
  const char *name() const noexcept override { return "lowpass"; }

private:
  std::size_t window_size_;
};

#endif