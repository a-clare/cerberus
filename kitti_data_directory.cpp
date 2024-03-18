#include "kitti_data_directory.h"
#include "logger.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/**
 * @brief A helper struct to hold a full path to some kitti sensor data
 * For example path might == "/some/path/to/image_03/data/000123.png"
 */
struct KittiSensorPath {
  static constexpr uint64_t KittiSensorPathSize = 1024;
  char path[KittiSensorPathSize];
};

struct KittiDataDir {
  const char* root;
  // The number of images in image_00/. Sensor name is LeftImgGrey
  uint32_t image00_cnt;
  // The number of images in image_01/. Sensor name is RightImgGrey
  uint32_t image01_cnt;
  // The number of images in image_02/. Sensor name is LeftImgRGB
  uint32_t image02_cnt;
  // The number of images in image_03/. Sensor name is RightImgRGB
  uint32_t image03_cnt;
  // The number of velodyne binary files in velodyne_points/. Sensor name is HDL64
  uint32_t velo_cnt;
};

/**
 * @return uint32_t the number of files of type "fileExt" ("txt" for example) in
 * root/folderName
 */
static uint32_t count_file_types(const char* root, const char* folderName,
                                 const char* fileExt);

/**
 * @brief Takes the path to some kitti data directory (root), a sensor name, and
 * a sequence number and creates a full path to the sensor data (path).
 */
static void sensor_name_to_path(const char* root,
                                KittiDataDirSensorNames name,
                                uint64_t seqNum,
                                KittiSensorPath* path);

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

  return KITTI_DATA_DIR_ERRORS_NONE;
}

void kitti_data_dir_free(KittiDataDirPtr dir) {
  free(dir);
}

void kitti_data_dir_load_img(KittiDataDirPtr dir,
                             uint32_t sensorNum,
                             uint32_t imgNum,
                             StereoImage* stereoImage) {
  char buf[1024];
  snprintf(buf, 1024, "%s/%s/data/%010d.png", dir->root, "image_00", imgNum);
  LOG("Loading image %s", buf);
  int width, height, num_channels;
  stereoImage->left = stbi_load(buf, &width, &height, &num_channels, 1);
  snprintf(buf, 1024, "%s/%s/data/%010d.png", dir->root, "image_01", imgNum);
  stereoImage->left = stbi_load(buf, &width, &height, &num_channels, 1);
  stereoImage->width = width;
  stereoImage->height = height;
  LOG("Image size is %d x %d", stereoImage->width, stereoImage->height);
}

KittiDataDirErrors kiti_data_dir_load_img_u8(KittiDataDirPtr dir,
                                             KittiDataDirSensorNames sensorName,
                                             uint32_t seqNum,
                                             ImageU8* img) {
  if (dir == NULL || img == NULL) {
    return KITTI_DATA_DIR_ERRORS_NULL_INPUT;
  }
  // Since we are loading a single channel image, we expect the sensor name to
  // be one of the grey scale images
  if (sensorName != KittiDataDirSensorNames::LeftImgGrey && sensorName != KittiDataDirSensorNames::RightImgGrey) {
    return KITTI_DATA_DIR_ERRORS_INVALID_SENSOR_NAME;
  }
  if (seqNum > dir->image00_cnt || seqNum > dir->image01_cnt) {
    return KITTI_DATA_DIR_ERRORS_INVALID_SEQ_NUM;
  }
  KittiSensorPath sensor_path;
  sensor_name_to_path(dir->root, sensorName, seqNum, &sensor_path);

  
  return KITTI_DATA_DIR_ERRORS_NONE;
}

uint32_t count_file_types(const char* root, 
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

void sensor_name_to_path(const char* root,
                                KittiDataDirSensorNames name,
                                uint64_t seqNum,
                                KittiSensorPath* path) {
  static const char* sensor_path_names[5] = {
    "image_00/",
    "image_01/",
    "image_02/",
    "image_03/",
    "velodyne_points/"
  };
  static const char* sensor_exts[2] = {
    // All images are stored as *.png files
    "png",
    // Velodyne points are stored as *.bin files
    "bin"
  };
  // Pointer to the string in sensor_path_names that will be our input
  // sensor name, need to check 'name' before can deterime the string
  const char* sensor_name;
  // Pointer to the file ext
  const char* sensor_ext;

  switch (name) {
    case KittiDataDirSensorNames::LeftImgGrey:
      sensor_name = sensor_path_names[0];
      sensor_ext  = sensor_exts[0]; 
      break;
    case KittiDataDirSensorNames::RightImgGrey:
      sensor_name = sensor_path_names[1];
      sensor_ext  = sensor_exts[0];
      break;
    case KittiDataDirSensorNames::LeftImgRGB:
      sensor_name = sensor_path_names[2];
      sensor_ext  = sensor_exts[0];
      break;
    case KittiDataDirSensorNames::RightImgRGB:
      sensor_name = sensor_path_names[4];
      sensor_ext  = sensor_exts[0];
      break;
    case KittiDataDirSensorNames::HDL64:
      sensor_name = sensor_path_names[5];
      sensor_ext  = sensor_exts[1];
      break;
    default:
      // TODO: Add assert? Although its hard to see how we would get an unexpected enum?
      break;
  }
  // This is going to create (for example) /some/path/that/is/root/image_00/data/0000000000.png
  snprintf(path->path, 
           path->KittiSensorPathSize,
           "%s/%s/data/%010lu.%s", 
           root,
           sensor_name,
           seqNum,
           sensor_ext);
  LOG("Loading path %s", path->path);
}