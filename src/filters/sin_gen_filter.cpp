#include "filters/sin_gen_filter.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>

SinGeneratorFilter::SinGeneratorFilter(double frequency_hz, double duration_ms)
    : frequency_hz_(frequency_hz), duration_ms_(duration_ms) {}

void SinGeneratorFilter::apply(Waveform &waveform) const {
  std::size_t sample_count =
      static_cast<std::size_t>((duration_ms_ / 1000.0) * Waveform::SampleRate);
  if (sample_count == 0) {
    return;
  }

  std::vector<int16_t> generated_samples(sample_count);
  const double two_pi = 6.283185307179586;
  const double sample_rate = static_cast<double>(Waveform::SampleRate);

  for (std::size_t sample_index = 0; sample_index < sample_count;
       ++sample_index) {
    double time_seconds = static_cast<double>(sample_index) / sample_rate;
    double sample_value =
        32767.0 * std::sin(two_pi * frequency_hz_ * time_seconds);
    generated_samples[sample_index] = static_cast<int16_t>(
        std::clamp(sample_value, static_cast<double>(INT16_MIN),
                   static_cast<double>(INT16_MAX)));
  }

  waveform.samples() = std::move(generated_samples);
}