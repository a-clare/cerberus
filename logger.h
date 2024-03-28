#ifndef CERBERUS_LOGGER_H_
#define CERBERUS_LOGGER_H_

#include <stdio.h>

#define LOG(fmt, ...) \
  do { \
    printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
  } while (0)

#endif