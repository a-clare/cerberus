#include "kitti_data_directory.h"
#include "logger.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct KittiDataDir {
  const char* root;
  // The number of images in image_00/
  uint32_t image00_cnt;
  // The number of images in image_01/
  uint32_t image01_cnt;
  // The number of images in image_02/
  uint32_t image02_cnt;
  // The number of images in image_03/
  uint32_t image03_cnt;
  // The number of velodyne binary files in velodyne_points/
  uint32_t velo_cnt;
};

/**
 * @return uint32_t the number of files of type "fileExt" ("txt" for example) in
 * root/folderName
 */
static uint32_t count_file_types(const char* root, const char* folderName,
                                 const char* fileExt);

KittiDataDirErrors kitti_data_dir_create(const char* root,
                                         KittiDataDirPtr* dir) {
  if (root == NULL || dir == NULL) {
    return KITTI_DATA_DIR_ERRORS_NULL_INPUT;
  }
  *dir = (KittiDataDirPtr)malloc(sizeof(KittiDataDir));
  if (*dir == NULL) {
    return KITTI_DATA_DIR_ERRORS_ALLOC;
  }

  DIR* directory = opendir(root);

  if (directory == NULL) {
    free(*dir);
    return KITTI_DATA_DIR_ERRORS_INVALID_DIR;
  }

  // We are looking for some directory with the name /image_00/, /image_01/,
  // /image_02/, /image_03 So if we get a count of 4 image directories we assume
  // we found them all
  int found_all_image_dirs = 0;
  bool found_velodyne_points = false;
  struct dirent* entry;
  // Read directory entries
  while ((entry = readdir(directory)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      if (strcmp(entry->d_name, "image_00") == 0) {
        found_all_image_dirs += 1;
      } else if (strcmp(entry->d_name, "image_01") == 0) {
        found_all_image_dirs += 1;
      } else if (strcmp(entry->d_name, "image_02") == 0) {
        found_all_image_dirs += 1;
      } else if (strcmp(entry->d_name, "image_03") == 0) {
        found_all_image_dirs += 1;
      } else if (strcmp(entry->d_name, "velodyne_points") == 0) {
        found_velodyne_points = true;
      }
    }
  }
  if (found_all_image_dirs != 4) {
    free(*dir);
    LOG("Only found %d image directories", found_all_image_dirs);
    return KITTI_DATA_DIR_ERRORS_NO_IMAGE_DIR;
  }
  if (!found_velodyne_points) {
    free(*dir);
    return KITTI_DATA_DIR_ERRORS_NO_VELO_DIR;
  }
  // We got here so all checks passed, can initialize the object
  (*dir)->root = root;
  (*dir)->image00_cnt = count_file_types(root, "image_00/data/", "png");
  (*dir)->image01_cnt = count_file_types(root, "image_01/data/", "png");
  (*dir)->image02_cnt = count_file_types(root, "image_02/data/", "png");
  (*dir)->image03_cnt = count_file_types(root, "image_03/data/", "png");
  (*dir)->velo_cnt    = count_file_types(root, "velodyne_points/data/", "bin");

  LOG("Num 00 %d", (*dir)->image00_cnt);
  LOG("Num 01 %d", (*dir)->image01_cnt);
  LOG("Num 02 %d", (*dir)->image02_cnt);
  LOG("Num 03 %d", (*dir)->image03_cnt);
  return KITTI_DATA_DIR_ERRORS_NONE;
}

static uint32_t count_file_types(const char* root, 
                                 const char* folderName,
                                 const char* fileExt) {
  uint32_t count = 0;

  // Create the /full/folder/name/path
  char buf[1024];
  snprintf(buf, 1024, "%s/%s", root, folderName);

  DIR* directory = opendir(buf);
  struct dirent *entry;
  while ((entry = readdir(directory)) != NULL) {
    // Check if it's a regular file
    if (entry->d_type == DT_REG) {  
      // Check if the file has a .txt extension
      char* dot = strrchr(entry->d_name, '.');
      if (dot && strcmp(dot + 1, fileExt) == 0) {
        count++;
      }
    }
  }
  return count;
}