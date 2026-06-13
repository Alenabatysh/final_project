#include "filters/am_gen_filter.h"
#include <algorithm>
#include <climits>
#include <cmath>

AmGenFilter::AmGenFilter(double amplitude, double carrier_hz,
                         double modulation_hz, double depth, double duration_ms)
    : amplitude_(amplitude), carrier_hz_(carrier_hz),
      modulation_hz_(modulation_hz), depth_(depth), duration_ms_(duration_ms) {}

void AmGenFilter::apply(Waveform &waveform) const {
  std::size_t sample_count =
      static_cast<std::size_t>((duration_ms_ / 1000.0) * Waveform::SampleRate);
  if (sample_count == 0) {
    return;
  }

  std::vector<int16_t> samples(sample_count);
  const double two_pi = 6.283185307179586;
  const double sample_rate = static_cast<double>(Waveform::SampleRate);
  const double max_amplitude = 32767.0;

  for (std::size_t index = 0; index < sample_count; ++index) {
    double time_seconds = static_cast<double>(index) / sample_rate;
    double envelope =
        1.0 + depth_ * std::sin(two_pi * modulation_hz_ * time_seconds);
    double carrier = std::sin(two_pi * carrier_hz_ * time_seconds);
    double sample_value = amplitude_ * max_amplitude * envelope * carrier;

    if (sample_value > max_amplitude) {
      sample_value = max_amplitude;
    }
    if (sample_value < -max_amplitude) {
      sample_value = -max_amplitude;
    }

    samples[index] = static_cast<int16_t>(std::round(sample_value));
  }

  waveform.samples() = std::move(samples);
}