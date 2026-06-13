#include "filters/silence_filter.h"
#include <algorithm>
#include <cstddef>

SilenceFilter::SilenceFilter(double start_seconds, double end_seconds)
    : start_seconds_(start_seconds), end_seconds_(end_seconds) {}

void SilenceFilter::apply(Waveform &wav) const {
  auto &samples = wav.samples();
  std::size_t start = wav.seconds_to_sample(start_seconds_);
  std::size_t end = wav.seconds_to_sample(end_seconds_);
  if (start >= end)
    return;
  std::size_t silence_count = end - start;
  samples.insert(samples.begin() + static_cast<std::ptrdiff_t>(start),
                 silence_count, 0);
}