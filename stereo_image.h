#ifndef CERBERUS_STEREO_IMAGE_H_
#define CERBERUS_STEREO_IMAGE_H_

#include <stdint.h>

typedef struct {
  // Time of the images in nanoseconds
  uint64_t timestamp;
  // Width of a single image, assumes left and right are the same size
  uint32_t width;
  // Height of a single image, assumes left and right are the same size
  uint32_t height;
  unsigned char* left;
  unsigned char* right;
} StereoImage;

#endif