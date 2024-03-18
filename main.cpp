#include <dirent.h>
#include <string.h>
#include <glad/glad.h>

#include <cstdio>
#include "kitti_data_directory.h"
#include "logger.h"
#include "image_window.h"
#include "vis/vis.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    LOG("%s", "No input directory provided");
    return 0;
  }

  vis_init("Cerberus", 1200, 1080);
  const char *input_dir = argv[1];
  LOG("Input directory is %s", input_dir);

  KittiDataDirPtr ptr;
  KittiDataDirErrors err = kitti_data_dir_create(input_dir, &ptr);
  if (err != KITTI_DATA_DIR_ERRORS_NONE) {
    LOG("Kitti data directory error %d", err);
    return 0;
  }
  ImageU8 left_image;
  kiti_data_dir_load_img_u8(ptr, KittiDataDirSensorNames::LeftImgGrey, 0, &left_image);
  ImageU8 right_image;
  kiti_data_dir_load_img_u8(ptr, KittiDataDirSensorNames::RightImgGrey, 0, &right_image);

  ImageWindowPtr left_window = image_window_create(&left_image);
  ImageWindowPtr right_window = image_window_create(&right_image);
  while (vis_is_window_running()) {
    vis_frame_start();

    image_window_draw(left_window, "left");
    image_window_draw(right_window, "right");
    vis_frame_end();
  }
  return 0;
}