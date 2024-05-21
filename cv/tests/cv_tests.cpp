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

  const uint64_t true_rows = 370;
  const uint64_t true_cols = 1224;
  const uint64_t true_size = true_rows * true_cols;

  auto true_img_path = CV_UNIT_TEST_DATA_DIR + "/true_grey.bin";
  auto true_img = LoadTrueBinaryImageData(true_img_path.c_str());
  ASSERT_TRUE(true_img.size() != 0) << "Failed to load (does the file exist?) " << true_img_path;
  ASSERT_TRUE(true_img.size() == true_size) << true_img_path << " is size " << true_img.size() <<
    " and expected size of " << true_size;

  uint8_t* data = img->data;

  for (uint32_t i = 0; i < true_size; i++) {
    ASSERT_EQ(data[i], true_img[i]) << "i " << i;
  }
}