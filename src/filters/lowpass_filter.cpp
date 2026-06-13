#include "filters/lowpass_filter.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <cmath>

LowpassFilter::LowpassFilter(std::size_t window_size)
    : window_size_(window_size) {
  if (window_size_ == 0 || window_size_ % 2 == 0) {
    throw std::invalid_argument("Lowpass window size must be odd and positive");
  }
}

void LowpassFilter::apply(Waveform &waveform) const {
  const auto &input_samples = waveform.samples();
  if (input_samples.empty()) {
    return;
  }

  const std::size_t half_window = window_size_ / 2;
  std::vector<int16_t> output_samples(input_samples.size());

  for (std::size_t sample_index = 0; sample_index < input_samples.size();
       ++sample_index) {
    long long sum_samples = 0;
    for (std::size_t offset = 0; offset < window_size_; ++offset) {
      long long neighbor_index = static_cast<long long>(sample_index) +
                                 static_cast<long long>(offset) -
                                 static_cast<long long>(half_window);
      neighbor_index =
          std::clamp(neighbor_index, 0LL,
                     static_cast<long long>(input_samples.size() - 1));
      sum_samples += input_samples[static_cast<std::size_t>(neighbor_index)];
    }
    double average_value =
        static_cast<double>(sum_samples) / static_cast<double>(window_size_);
    output_samples[sample_index] =
        static_cast<int16_t>(std::round(average_value));
  }

  waveform.samples() = std::move(output_samples);
}