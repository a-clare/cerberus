#ifndef CERBERUS_IMAGE_WINDOW_H_
#define CERBERUS_IMAGE_WINDOW_H_

#include "vis/vis.h"
#include "image.h"

typedef struct ImageWindow ImageWindow;
typedef struct ImageWindow* ImageWindowPtr;

ImageWindowPtr image_window_create(ImageU8* img);

void image_window_draw(const ImageWindowPtr win,
                       const char* windowName);

#endif