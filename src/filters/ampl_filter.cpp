#include "filters/ampl_filter.h"
#include <algorithm>
#include <climits>
#include <cmath>

void AmplFilter::apply(Waveform &waveform) const {
  auto &samples = waveform.samples();
  for (auto &sample : samples) {
    double new_value = static_cast<double>(sample) * factor_;
    double clamped = std::clamp(new_value, static_cast<double>(INT16_MIN),
                                static_cast<double>(INT16_MAX));
    sample = static_cast<int16_t>(std::round(clamped));
  }
}