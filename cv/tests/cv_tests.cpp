#include <gtest/gtest.h>
#include "cv/image.h"
#include "true_img_data.h"

#ifndef UNIT_TEST_DATA_DIR
assert("UNIT_TEST_DATA_DIR not defined, need this to find CV test data")
#endif

static const std::string CV_UNIT_TEST_DATA_DIR = std::string(UNIT_TEST_DATA_DIR) + "/cv/";

TEST(CV, ReadPNG) {
  cv_Image* img;
  auto err = cv_image_read((CV_UNIT_TEST_DATA_DIR + "/img_grey.png").c_str(), &img);
  EXPECT_EQ(err, CV_IMAGE_ERROR_NONE);
  EXPECT_EQ(img->height, 370);
  EXPECT_EQ(img->width, 1224);
  EXPECT_EQ(img->channels, 1);

  uint8_t* data = img->data;
  for (uint32_t i = 0; i < (370 * 1224); i++) {
    EXPECT_EQ(data[i], true_img_png_read[i]);
  }
}