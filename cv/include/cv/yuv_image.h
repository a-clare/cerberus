#ifndef CERBERUS_YUV_IMAGE_H_
#define CERBERUS_YUV_IMAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "cv/image.h"

typedef struct {
  cv_ImageSize size;
  uint8_t* data;
} cv_YUVImage;


void cv_yuv_image_create(cv_YUVImage** img);

#ifdef __cplusplus
}
#endif
#endif