#ifndef CERBERUS_IMAGE_H_
#define CERBERUS_IMAGE_H_

#include <cstdint>
#include <array>

static constexpr uint32_t IMAGEU8_MAX_WIDTH  = 1600;
static constexpr uint32_t IMAGEU8_MAX_HEIGHT = 1200;
static constexpr uint32_t IMAGEU8_MAX_SIZE = IMAGEU8_MAX_WIDTH * IMAGEU8_MAX_HEIGHT;

/**
 * @brief Single 8bit channel image (grey scale for example)
 * 
 */
struct ImageU8 {
  uint64_t timestamp = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  std::array<uint8_t, IMAGEU8_MAX_SIZE> image;
};

#endif