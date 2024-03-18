#include <gtest/gtest.h>
#include "image.h"
#include "true_img_data.h"

TEST(ImageU8, read) {
  // TODO: Replace this path with a test data directory
  ImageU8* img = image_u8_read("/media/atc/Disk1/data/kitti/2011_09_28/2011_09_28_drive_0002_sync/image_00/data/0000000000.png");

  EXPECT_EQ(img->height, 370);
  EXPECT_EQ(img->width, 1224);

  for (uint32_t i = 0; i < (370 * 1224); i++) {
    EXPECT_EQ(img->image[i], true_img_png_read[i]);
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}