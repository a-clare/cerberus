#ifndef CERBERUS_IMAGE_H_
#define CERBERUS_IMAGE_H_

#include <stdint.h>

typedef struct ImageU8 ImageU8;
typedef struct ImageU8* ImageU8Ptr;

typedef enum {
  IMAGE_ERRORS_NONE,
  IMAGE_ERRORS_READ
} ImageErrors;

typedef struct {
  uint32_t width;
  uint32_t height;
} ImageSize;

/**
 * @brief Read in a single channel 8 bit image
 * Allocates memory for img
 * 
 * @param path /some/path/to/an/image.png
 * @param img the return image
 * @return IMAGE_ERRORS_NONE if successful 
 * @return IMAGE_ERRORS_READ if unable to read, likely a bad path
 */
ImageErrors image_read_u8(const char* path,
                          ImageU8Ptr* img);

ImageSize image_get_size_u8(const ImageU8Ptr img);

uint8_t* image_get_image_data_u8(const ImageU8Ptr img);

void image_write(const ImageU8Ptr img);
#endif