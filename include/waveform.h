#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

class Waveform {
public:
  static constexpr int SampleRate = 44100;
  static constexpr int Channels = 1;
  static constexpr int BitsPerSample = 16;

  explicit Waveform(std::size_t num_samples = 0);
  Waveform(const std::vector<int16_t> &samples,
           std::uint32_t sample_rate = SampleRate);
  Waveform(std::vector<int16_t> &&samples,
           std::uint32_t sample_rate = SampleRate);
  Waveform(std::initializer_list<int16_t> samples,
           std::uint32_t sample_rate = SampleRate);

  Waveform(const Waveform &other);
  Waveform &operator=(const Waveform &other);
  Waveform(Waveform &&other) noexcept;
  Waveform &operator=(Waveform &&other) noexcept;

  std::size_t num_samples() const noexcept { return samples_.size(); }
  double duration_sec() const noexcept;
  double duration_ms() const noexcept;

  std::size_t seconds_to_sample(double seconds) const noexcept;
  std::size_t milliseconds_to_sample(double milliseconds) const noexcept;

  void resize(std::size_t new_num_samples);

  const std::vector<int16_t> &samples() const noexcept { return samples_; }
  std::vector<int16_t> &samples() noexcept { return samples_; }

  int16_t operator[](std::size_t index) const noexcept {
    return samples_[index];
  }
  int16_t &operator[](std::size_t index) noexcept { return samples_[index]; }

private:
  std::vector<int16_t> samples_;
  std::uint32_t sample_rate_ = SampleRate;
};

#endif