#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include "waveform.h"
#include "wav_structs.h"
#include <fstream>

class WavWriter {
public:
  void write(const char *filename, const Waveform &waveform);

private:
  RiffHeader buildRiffHeader(const Waveform &waveform,
                             std::size_t data_size) const noexcept;
  FmtHeader buildFmtHeader() const noexcept;
  DataHeader buildDataHeader(std::size_t data_size) const noexcept;
  void writeHeaders(std::ofstream &file, const RiffHeader &riff,
                    const FmtHeader &fmt, const DataHeader &data) const;
};

#endif