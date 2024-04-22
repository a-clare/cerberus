#include <gtest/gtest.h>
#include "cv/image.h"
#include "true_img_data.h"

TEST(CV, ReadPNG) {
  cv_Image* img;
  auto err = cv_image_read("/home/atc/hds/data/kitti/2011_09_28/2011_09_28_drive_0002_sync/image_00/data/0000000000.png", &img);
  EXPECT_EQ(err, CV_IMAGE_ERROR_NONE);
  EXPECT_EQ(img->height, 370);
  EXPECT_EQ(img->width, 1224);
  EXPECT_EQ(img->channels, 1);

  uint8_t* data = img->data;
  for (uint32_t i = 0; i < (370 * 1224); i++) {
    EXPECT_EQ(data[i], true_img_png_read[i]);
  }
}