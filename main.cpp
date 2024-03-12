#include <dirent.h>
#include <string.h>

#include <cstdio>

#include "logger.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    LOG("%s", "No input directory provided");
    return 0;
  }

  const char *input_dir = argv[1];
  LOG("Input directory is %s", input_dir);

  DIR *dir;
  struct dirent *entry;

  // Open the directory
  dir = opendir(input_dir);

  if (dir == NULL) {
    LOG("Could not open directory %s", input_dir);
    return 1;
  }

  // Read directory entries
  // We are looking for some directory with the name /image_00 and /image_01
  bool found_img_dir = false;
  // Read directory entries
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      // Check if the current entry is "image_00" or "image_01"
      if (strcmp(entry->d_name, "image_00") == 0 || strcmp(entry->d_name, "image_01") == 0) {
        found_img_dir = true;
        break;
      }
    }
  }
  if (!found_img_dir) {
    LOG("Could not find image_00 or image_01 in %s", input_dir);
    return 1;
  }

  // Close the directory
  closedir(dir);

  return 0;
}