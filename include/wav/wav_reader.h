#ifndef WAV_READER_H
#define WAV_READER_H

#include "wav_structs.h"
#include "waveform.h"
#include <fstream>

class WavReader {
public:
  Waveform read(const char *filename);

private:
  void readRiffHeader(std::ifstream &file, RiffHeader &header) const;
  void readFmtHeader(std::ifstream &file, FmtHeader &header) const;
  void findDataChunk(std::ifstream &file, DataHeader &header) const;
  Waveform readAudioData(std::ifstream &file, const FmtHeader &fmt_header,
                         const DataHeader &data_header) const;
};

#endif