#ifndef CERBERUS_CV_IMAGE_H_
#define CERBERUS_CV_IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Common struct that defines the image size parameters, and is used across all image types 
 */
typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t channels;
} cv_ImageSize;

#ifdef __cplusplus
}
#endif
#endif