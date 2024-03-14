#include <dirent.h>
#include <string.h>

#include <cstdio>
#include "kitti_data_directory.h"
#include "logger.h"
#include "vis/vis.h"

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

  kitti_data_dir_load_img(ptr, 0, 0);


  vis_init("Cerberus", 1200, 1080);
  while (vis_is_window_running()) {
    vis_frame_start();
    
    vis_frame_end();
  }
  return 0;
}