#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "filters/am_gen_filter.h"
#include "filters/ampl_filter.h"
#include "filters/fade_filter.h"
#include "filters/lowpass_filter.h"
#include "filters/normalize_filter.h"
#include "filters/reverse_filter.h"
#include "filters/silence_filter.h"
#include "filters/sin_gen_filter.h"
#include "filters/timestretch_filter.h"
#include "waveform.h"

#include <cmath>

TEST_CASE("Waveform basic operations", "[waveform]") {
  Waveform waveform(100);
  REQUIRE(waveform.num_samples() == 100);
  REQUIRE(waveform.duration_sec() ==
          Catch::Approx(100.0 / Waveform::SampleRate));
  waveform[0] = 12345;
  REQUIRE(waveform[0] == 12345);
}

TEST_CASE("AmplFilter applies gain", "[ampl]") {
  Waveform waveform(3);
  waveform[0] = 1000;
  waveform[1] = -2000;
  waveform[2] = 30000;
  AmplFilter filter(2.0);
  filter.apply(waveform);
  REQUIRE(waveform[0] == 2000);
  REQUIRE(waveform[1] == -4000);
  REQUIRE(waveform[2] == 32767);
}

TEST_CASE("SilenceFilter inserts zeros", "[silence]") {
  const double start_seconds = 0.1;
  const double end_seconds = 0.2;
  Waveform waveform(44100);
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    waveform[index] = 1;
  }

  SilenceFilter filter(start_seconds, end_seconds);
  std::size_t start_index = waveform.seconds_to_sample(start_seconds);
  std::size_t end_index = waveform.seconds_to_sample(end_seconds);
  std::size_t silence_count = end_index - start_index;

  filter.apply(waveform);
  for (std::size_t index = start_index; index < start_index + silence_count;
       ++index) {
    REQUIRE(waveform[index] == 0);
  }
  REQUIRE(waveform.num_samples() == 44100 + silence_count);
}

TEST_CASE("TimestretchFilter changes length", "[timestretch]") {
  Waveform waveform(1000);
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    waveform[index] = static_cast<int16_t>(index);
  }
  TimestretchFilter filter(2.0);
  filter.apply(waveform);
  REQUIRE(waveform.num_samples() == 2000);
}

TEST_CASE("SinGeneratorFilter creates sine wave", "[sin_gen]") {
  const double frequency_hz = 440.0;
  const double duration_ms = 1000.0;
  SinGeneratorFilter filter(frequency_hz, duration_ms);
  Waveform waveform;
  filter.apply(waveform);
  REQUIRE(
      waveform.num_samples() ==
      static_cast<std::size_t>(Waveform::SampleRate * duration_ms / 1000.0));
  REQUIRE(waveform[0] == 0);
  std::size_t quarter_samples =
      static_cast<std::size_t>(Waveform::SampleRate / frequency_hz / 4);
  if (quarter_samples < waveform.num_samples()) {
    REQUIRE(waveform[quarter_samples] > 0);
  }
}

TEST_CASE("NormalizeFilter scales to target peak", "[normalize]") {
  Waveform waveform({1000, -2000, 3000});
  NormalizeFilter filter(0.5);
  filter.apply(waveform);
  double target_peak = 0.5 * 32767.0;
  double current_peak = 3000.0;
  double scale = target_peak / current_peak;
  REQUIRE(waveform[0] == Catch::Approx(1000.0 * scale).epsilon(0.01));
  REQUIRE(waveform[1] == Catch::Approx(-2000.0 * scale).epsilon(0.01));
  REQUIRE(waveform[2] == Catch::Approx(3000.0 * scale).epsilon(0.01));
}

TEST_CASE("ReverseFilter reverses samples", "[reverse]") {
  Waveform waveform(5);
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    waveform[index] = static_cast<int16_t>(index);
  }
  ReverseFilter filter;
  filter.apply(waveform);
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    REQUIRE(waveform[index] ==
            static_cast<int16_t>(waveform.num_samples() - 1 - index));
  }
}

TEST_CASE("LowpassFilter smooths signal", "[lowpass]") {
  Waveform waveform({100, 200, 100, 200, 100});
  LowpassFilter filter(3);
  filter.apply(waveform);
  REQUIRE(waveform[1] == 133);
  REQUIRE(waveform[2] == 167);
}

TEST_CASE("FadeFilter in/out", "[fade]") {
  const double duration_ms = 500.0;
  const double half_second = 0.5;
  Waveform waveform(Waveform::SampleRate);
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    waveform[index] = 32767;
  }

  FadeFilter fade_in(FadeFilter::Mode::in, duration_ms);
  fade_in.apply(waveform);
  REQUIRE(waveform[0] == 0);
  REQUIRE(waveform[waveform.seconds_to_sample(0.45)] < 32767);
  REQUIRE(waveform[waveform.seconds_to_sample(half_second)] == 32767);

  Waveform waveform2(Waveform::SampleRate);
  for (std::size_t index = 0; index < waveform2.num_samples(); ++index) {
    waveform2[index] = 32767;
  }
  FadeFilter fade_out(FadeFilter::Mode::out, duration_ms);
  fade_out.apply(waveform2);
  REQUIRE(waveform2[waveform2.seconds_to_sample(half_second)] == 32767);
  REQUIRE(waveform2[waveform2.num_samples() - 1] == 0);
}

TEST_CASE("AmGenFilter creates amplitude‑modulated sine", "[am_gen]") {
  AmGenFilter filter(0.8, 440.0, 5.0, 0.5, 1000.0);
  Waveform waveform;
  filter.apply(waveform);
  REQUIRE(waveform.num_samples() == 44100);

  bool has_non_zero = false;
  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    if (waveform[index] != 0) {
      has_non_zero = true;
      break;
    }
  }
  REQUIRE(has_non_zero);

  for (std::size_t index = 0; index < waveform.num_samples(); ++index) {
    REQUIRE(std::abs(waveform[index]) <= 32767);
  }
}