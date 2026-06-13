#include "waveform.h"
#include <algorithm>
#include <cmath>

Waveform::Waveform(std::size_t num_samples) : samples_(num_samples, 0) {}

Waveform::Waveform(const std::vector<int16_t> &samples,
                   std::uint32_t sample_rate)
    : samples_(samples), sample_rate_(sample_rate) {}

Waveform::Waveform(std::vector<int16_t> &&samples, std::uint32_t sample_rate)
    : samples_(std::move(samples)), sample_rate_(sample_rate) {}

Waveform::Waveform(std::initializer_list<int16_t> samples,
                   std::uint32_t sample_rate)
    : samples_(samples), sample_rate_(sample_rate) {}

Waveform::Waveform(const Waveform &other) = default;
Waveform &Waveform::operator=(const Waveform &other) = default;
Waveform::Waveform(Waveform &&other) noexcept = default;
Waveform &Waveform::operator=(Waveform &&other) noexcept = default;

double Waveform::duration_sec() const noexcept {
  return static_cast<double>(samples_.size()) /
         static_cast<double>(sample_rate_);
}

double Waveform::duration_ms() const noexcept {
  return duration_sec() * 1000.0;
}

std::size_t Waveform::seconds_to_sample(double seconds) const noexcept {
  if (seconds <= 0.0)
    return 0;
  double idx = seconds * static_cast<double>(sample_rate_);
  std::size_t total = samples_.size();
  if (idx >= static_cast<double>(total))
    return total;
  return static_cast<std::size_t>(idx);
}

std::size_t
Waveform::milliseconds_to_sample(double milliseconds) const noexcept {
  return seconds_to_sample(milliseconds / 1000.0);
}

void Waveform::resize(std::size_t new_num_samples) {
  samples_.resize(new_num_samples, 0);
}