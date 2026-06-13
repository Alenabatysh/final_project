#ifndef FADE_FILTER_H
#define FADE_FILTER_H

#include "ifilter.h"

class FadeFilter final : public IFilter {
public:
  enum class Mode { in, out };

  FadeFilter(Mode mode, double duration_ms);

  void apply(Waveform &waveform) const override;
  
  const char *name() const noexcept override {
    return (mode_ == Mode::in) ? "fade_in" : "fade_out";
  }

private:
  Mode mode_;
  double duration_ms_;
};

#endif