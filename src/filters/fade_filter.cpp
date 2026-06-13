#include "filters/fade_filter.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

FadeFilter::FadeFilter(Mode mode, double duration_ms)
    : mode_(mode), duration_ms_(duration_ms) {
  if (duration_ms_ < 0.0) {
    throw std::invalid_argument("Fade duration must be non-negative");
  }
}

void FadeFilter::apply(Waveform &waveform) const {
  auto &samples = waveform.samples();
  if (samples.empty()) {
    return;
  }

  std::size_t fade_samples =
      static_cast<std::size_t>((duration_ms_ / 1000.0) * Waveform::SampleRate);
  if (fade_samples == 0) {
    return;
  }
  if (fade_samples > samples.size()) {
    fade_samples = samples.size();
  }

  for (std::size_t index = 0; index < fade_samples; ++index) {
    double factor =
        static_cast<double>(index) / static_cast<double>(fade_samples - 1);
    if (mode_ == Mode::in) {
      samples[index] = static_cast<int16_t>(
          std::round(static_cast<double>(samples[index]) * factor));
    } else { // fade out
      std::size_t target_index = samples.size() - fade_samples + index;
      samples[target_index] = static_cast<int16_t>(std::round(
          static_cast<double>(samples[target_index]) * (1.0 - factor)));
    }
  }
}