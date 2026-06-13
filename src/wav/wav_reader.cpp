#include "wav/wav_reader.h"
#include "wav/wav_structs.h"
#include <fstream>
#include <stdexcept>

void WavReader::readRiffHeader(std::ifstream &file, RiffHeader &header) const {
  file.read(reinterpret_cast<char *>(&header), sizeof(header));
  if (!file) {
    throw std::runtime_error("Failed to read RIFF header");
  }
  if (header.chunk_id != RiffTag || header.format != WaveTag) {
    throw std::runtime_error("Not a valid WAVE file");
  }
}

void WavReader::readFmtHeader(std::ifstream &file, FmtHeader &header) const {
  file.read(reinterpret_cast<char *>(&header), sizeof(header));
  if (!file) {
    throw std::runtime_error("Failed to read fmt header");
  }
  if (header.subchunk_id != FmtTag) {
    throw std::runtime_error("fmt subchunk not found");
  }
  if (header.audio_format != 1) {
    throw std::runtime_error("Only PCM format is supported");
  }
  if (header.num_channels != 1) {
    throw std::runtime_error("Only mono (1 channel) is supported");
  }
  if (header.sample_rate != Waveform::SampleRate) {
    throw std::runtime_error("Sample rate must be 44100 Hz");
  }
  if (header.bits_per_sample != 16) {
    throw std::runtime_error("Bits per sample must be 16");
  }
}

void WavReader::findDataChunk(std::ifstream &file, DataHeader &header) const {
  bool data_found = false;
  while (!data_found && file) {
    std::streampos position = file.tellg();
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!file)
      break;

    if (header.subchunk_id == DataTag) {
      data_found = true;
      break;
    } else {
      std::streamoff skip_bytes = static_cast<std::streamoff>(
          header.subchunk_size + sizeof(DataHeader));
      file.seekg(position + skip_bytes);
    }
  }
  if (!data_found) {
    throw std::runtime_error("Data chunk not found");
  }
}

Waveform WavReader::readAudioData(std::ifstream &file,
                                  const FmtHeader &fmt_header,
                                  const DataHeader &data_header) const {
  std::size_t bytes_per_sample = fmt_header.bits_per_sample / 8;
  std::size_t num_samples = data_header.subchunk_size / bytes_per_sample;
  Waveform waveform(num_samples);

  file.read(reinterpret_cast<char *>(waveform.samples().data()),
            static_cast<std::streamsize>(data_header.subchunk_size));
  if (!file) {
    throw std::runtime_error("Failed to read audio data");
  }
  return waveform;
}

Waveform WavReader::read(const char *filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Cannot open input file");
  }

  RiffHeader riff_header;
  readRiffHeader(file, riff_header);

  FmtHeader fmt_header;
  readFmtHeader(file, fmt_header);

  DataHeader data_header;
  findDataChunk(file, data_header);

  return readAudioData(file, fmt_header, data_header);
}