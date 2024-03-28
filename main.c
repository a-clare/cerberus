#include <dirent.h>
#include <string.h>

#include <stdio.h>
#include "kitti_data_directory.h"
#include "logger.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    LOG("%s", "No input directory provided");
    return 0;
  }

  const char *input_dir = argv[1];
  LOG("Input directory is %s", input_dir);

  KittiDataDirPtr ptr;
  KittiDataDirErrors err = kitti_data_dir_create(input_dir, &ptr);
  if (err != KITTI_DATA_DIR_ERRORS_NONE) {
    LOG("Kitti data directory error %d", err);
    return 0;
  }
  ImageU8Ptr left_image;
  kiti_data_dir_load_img_u8(ptr, KITII_DATA_DIR_LeftImgGrey, 0, &left_image);
  ImageU8Ptr right_image;
  kiti_data_dir_load_img_u8(ptr, KITII_DATA_DIR_RightImgGrey, 0, &right_image);

  image_write(left_image);
  return 0;
}