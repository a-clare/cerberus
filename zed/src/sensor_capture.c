#include "zed/sensor_capture.h"
#include <stdlib.h>

struct zed_SensorCapture {

};

ZED_ERRORS zed_sc_create(zed_SensorCapture** zsc) {
  // The poitner that **zsc points to is allowed to be NULL (it means the
  // object is not allocated, which is what we are doing here in create()).
  // Hence that check is not needed, but we do need to make sure that **zsc 
  // is a valid pointer
  if (zsc == NULL) {
    return ZED_ERRORS_NULL_INPUT;
  }

  *zsc = (zed_SensorCapture*)malloc(sizeof(struct zed_SensorCapture));
  if (*zsc == NULL) {
    return ZED_ERRORS_ALLOC_FAIL;
  }

  return ZED_ERRORS_NONE;
}

ZED_ERRORS zed_sc_destroy(zed_SensorCapture** zsc) {
  if (zsc == NULL || *zsc == NULL) {
    return ZED_ERRORS_NULL_INPUT;
  }
  // Free the memory and make the input pointer NULL in case someone tries
  // to use it again after destroyed
  free(*zsc);
  *zsc = NULL;
  return ZED_ERRORS_NONE;
}