#include "filters/normalize_filter.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <stdexcept>

NormalizeFilter::NormalizeFilter(double peak) : peak_(peak) {
  if (peak < 0.0 || peak > 1.0) {
    throw std::invalid_argument("Normalize peak must be in [0, 1]");
  }
}

void NormalizeFilter::apply(Waveform &waveform) const {
  auto &samples = waveform.samples();
  if (samples.empty())
    return;

  int max_abs = 0;
  for (auto sample : samples) {
    int abs_val = std::abs(static_cast<int>(sample));
    if (abs_val > max_abs)
      max_abs = abs_val;
  }
  if (max_abs == 0)
    return;

  double scale =
      peak_ * static_cast<double>(INT16_MAX) / static_cast<double>(max_abs);
  for (auto &sample : samples) {
    double new_val = static_cast<double>(sample) * scale;
    sample =
        static_cast<int16_t>(std::clamp(new_val, static_cast<double>(INT16_MIN),
                                        static_cast<double>(INT16_MAX)));
  }
}