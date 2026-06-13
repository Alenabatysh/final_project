#ifndef IFILTER_H
#define IFILTER_H

#include "waveform.h"

class IFilter {
public:
  virtual ~IFilter() = default;

  virtual void apply(Waveform &waveform) const = 0;
  
  virtual const char *name() const noexcept = 0;
};

#endif