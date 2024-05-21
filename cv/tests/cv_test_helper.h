#ifndef CV_TEST_HELPER_H_
#define CV_TEST_HELPER_H_

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

/**
 * @brief Loads in a *.bin data that contains the true image rgb, or greyscale
 * data The true data is generated from matlab/image_read_write.m Only used for
 * unit tests so not going for efficiency on implementation Returns an empty
 * vector if failed to load data
 * @param filename "/some/path/to/a/binary/image/data/to/load"
 * @return std::vector<std::uint8_t>
 */
std::vector<std::uint8_t> LoadTrueBinaryImageData(const char* filename) {
  std::vector<std::uint8_t> ret;
  FILE* file = fopen(filename, "rb");
  if (file == nullptr) {
    return ret;
  }
  // Move the file pointer to the end of the file to determine its size
  fseek(file, 0, SEEK_END);
  auto file_size = ftell(file);
  fseek(file, 0, SEEK_SET);  // Move the file pointer back to the beginning of the file

  // Create a vector to hold the file contents
  ret.resize(file_size);
  // Read the file contents into the vector
  size_t bytes_read = fread(ret.data(), 1, file_size, file);
  if (bytes_read != file_size) {
    std::cerr << "Error reading file: " << filename << std::endl;
    fclose(file);
    // Cant return ret cause its resized, and we want to return a 0 size vector
    return std::vector<std::uint8_t>();
  }
  fclose(file);
  return ret;
}

#endif