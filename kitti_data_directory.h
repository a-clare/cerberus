#ifndef CERBERUS_KITTI_DATA_DIRECTORY_H_
#define CERBERUS_KITTI_DATA_DIRECTORY_H_

#include <stdint.h>
#include "stereo_image.h"

typedef struct KittiDataDir KittiDataDir;
// We will almost exclusively work with pointers to data directories, so 
// providing a typedef to make things a bit clearer
typedef struct KittiDataDir* KittiDataDirPtr;

typedef enum {
  KITTI_DATA_DIR_ERRORS_NONE,
  KITTI_DATA_DIR_ERRORS_NULL_INPUT,
  KITTI_DATA_DIR_ERRORS_ALLOC,
  KITTI_DATA_DIR_ERRORS_INVALID_DIR,
  KITTI_DATA_DIR_ERRORS_NO_IMAGE_DIR,
  KITTI_DATA_DIR_ERRORS_NO_VELO_DIR,
} KittiDataDirErrors;

/**
 * @brief Create a new kitti data directory object.
 * 
 * Will allocate memory for the data directory, up to caller to manage pointer lifetime.
 * Will check for validity by looking for expected folders within root.
 * 
 * @param root /some/path/to/a/kitti/data/dir, ".../2011_09_28/2011_09_28_drive_0001_sync" for example
 * @param dir the allocated directory object
 * @return KITTI_DATA_DIR_ERRORS_NONE if succesful
 * @return KITTI_DATA_DIR_ERRORS_NULL_INPUT if root or dir is null
 * @return KITTI_DATA_DIR_ERRORS_ALLOC if unable to allocate object
 * @return KITTI_DATA_DIR_ERRORS_INVALID_DIR if dir is not a directory (or does not exsist)
 * @return KITTI_DATA_DIR_ERRORS_NO_IMAGE_DIR if unable to find image_00, image_01, image_02, or image_03 directories
 * @return KITTI_DATA_DIR_ERRORS_NO_VELO_DIR if unable to find velodyne_points directory
 */
KittiDataDirErrors kitti_data_dir_create(const char* root,
                                         KittiDataDirPtr* dir);

/**
 * @brief frees the memory allocated for dir
 */
void kitti_data_dir_free(KittiDataDirPtr dir);

/**
 * @brief Load an image from the data directory
 * 
 * @param dir 
 * @param sensorNum 0, 1, 2, 3 for image_00, image_01, etc...
 * @param imgNum image (orsequence) number to load
 */
void kitti_data_dir_load_img(KittiDataDirPtr dir,
                             uint32_t sensorNum,
                             uint32_t imgNum,
                             StereoImage* stereoImage);
#endif