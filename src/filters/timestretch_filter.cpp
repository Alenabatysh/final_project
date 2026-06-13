#include "filters/timestretch_filter.h"
#include <cmath>
#include <vector>

TimestretchFilter::TimestretchFilter(double factor) : factor_(factor) {}

void TimestretchFilter::apply(Waveform &waveform) const {
  const auto &input_samples = waveform.samples();
  if (input_samples.empty()) {
    return;
  }

  std::size_t new_sample_count = static_cast<std::size_t>(
      std::round(static_cast<double>(input_samples.size()) * factor_));
  if (new_sample_count == 0) {
    waveform.resize(0);
    return;
  }

  std::vector<int16_t> output_samples(new_sample_count);

  for (std::size_t output_index = 0; output_index < new_sample_count;
       ++output_index) {
    double input_position = static_cast<double>(output_index) / factor_;
    std::size_t left_index = static_cast<std::size_t>(input_position);
    double fraction = input_position - static_cast<double>(left_index);

    if (left_index + 1 < input_samples.size()) {
      double interpolated_value =
          static_cast<double>(input_samples[left_index]) * (1.0 - fraction) +
          static_cast<double>(input_samples[left_index + 1]) * fraction;
      output_samples[output_index] =
          static_cast<int16_t>(std::round(interpolated_value));
    } else {
      output_samples[output_index] =
          input_samples[std::min(left_index, input_samples.size() - 1)];
    }
  }

  waveform.samples() = std::move(output_samples);
}