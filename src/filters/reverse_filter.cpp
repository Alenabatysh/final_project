#include "filters/reverse_filter.h"
#include <algorithm>

void ReverseFilter::apply(Waveform &waveform) const {
  auto &samples = waveform.samples();
  std::reverse(samples.begin(), samples.end());
}