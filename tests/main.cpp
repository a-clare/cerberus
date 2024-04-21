#include <gtest/gtest.h>
// extern "C" {
// #include "image.h"
// #include "true_img_data.h"
// }
// TEST(ImageU8, read) {
//   // TODO: Replace this path with a test data directory
//   ImageU8Ptr img;

//   EXPECT_TRUE(image_read_u8("/media/atc/Disk1/data/kitti/2011_09_28/2011_09_28_drive_0002_sync/image_00/data/0000000000.png", &img) == IMAGE_ERRORS_NONE);

//   auto s = image_get_size_u8(img);
//   EXPECT_EQ(s.height, 370);
//   EXPECT_EQ(s.width, 1224);

//   uint8_t* data = image_get_image_data_u8(img);
//   for (uint32_t i = 0; i < (370 * 1224); i++) {
//     EXPECT_EQ(data[i], true_img_png_read[i]);
//   }
// }

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}