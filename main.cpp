#include <dirent.h>
#include <string.h>

#include <cstdio>
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
  LOG("Error %d\n", err);
  return 0;
}