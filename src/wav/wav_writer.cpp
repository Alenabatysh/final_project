#include "wav/wav_writer.h"
#include "wav/wav_structs.h"
#include <fstream>
#include <stdexcept>

RiffHeader WavWriter::buildRiffHeader(const Waveform &waveform,
                                      std::size_t data_size) const noexcept {
  RiffHeader header;
  header.chunk_id = RiffTag;
  header.chunk_size =
      36 + static_cast<uint32_t>(data_size);
  header.format = WaveTag;
  return header;
}

FmtHeader WavWriter::buildFmtHeader() const noexcept {
  FmtHeader header;
  header.subchunk_id = FmtTag;
  header.subchunk_size = 16;
  header.audio_format = 1;
  header.num_channels = Waveform::Channels;
  header.sample_rate = Waveform::SampleRate;
  header.bits_per_sample = Waveform::BitsPerSample;
  header.byte_rate =
      Waveform::SampleRate * Waveform::Channels * Waveform::BitsPerSample / 8;
  header.block_align = Waveform::Channels * Waveform::BitsPerSample / 8;
  return header;
}

DataHeader WavWriter::buildDataHeader(std::size_t data_size) const noexcept {
  DataHeader header;
  header.subchunk_id = DataTag;
  header.subchunk_size = static_cast<uint32_t>(data_size);
  return header;
}

void WavWriter::writeHeaders(std::ofstream &file, const RiffHeader &riff,
                             const FmtHeader &fmt,
                             const DataHeader &data) const {
  file.write(reinterpret_cast<const char *>(&riff), sizeof(riff));
  file.write(reinterpret_cast<const char *>(&fmt), sizeof(fmt));
  file.write(reinterpret_cast<const char *>(&data), sizeof(data));
}

void WavWriter::write(const char *filename, const Waveform &waveform) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Cannot create output file");
  }

  std::size_t num_samples = waveform.num_samples();
  std::size_t data_size = num_samples * (Waveform::BitsPerSample / 8);

  RiffHeader riff_header = buildRiffHeader(waveform, data_size);
  FmtHeader fmt_header = buildFmtHeader();
  DataHeader data_header = buildDataHeader(data_size);

  writeHeaders(file, riff_header, fmt_header, data_header);

  file.write(reinterpret_cast<const char *>(waveform.samples().data()),
             static_cast<std::streamsize>(data_size));

  if (!file) {
    throw std::runtime_error("Failed to write audio data");
  }
}