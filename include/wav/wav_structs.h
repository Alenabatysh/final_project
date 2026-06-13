#ifndef WAV_STRUCTS_H
#define WAV_STRUCTS_H

#include <cstdint>

#pragma pack(push, 1)

struct RiffHeader {
  uint32_t chunk_id;
  uint32_t chunk_size;
  uint32_t format;
};

struct FmtHeader {
  uint32_t subchunk_id;
  uint32_t subchunk_size;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
};

struct DataHeader {
  uint32_t subchunk_id;
  uint32_t subchunk_size;
};

#pragma pack(pop)

constexpr uint32_t RiffTag = 0x46464952;
constexpr uint32_t WaveTag = 0x45564157;
constexpr uint32_t FmtTag = 0x20746D66;
constexpr uint32_t DataTag = 0x61746164;

#endif