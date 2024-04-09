#include "zed/sensor_capture.h"
#include <stdio.h>

int main() {
  zed_SensorCapture* zsc;
  ZED_ERRORS zed_err = zed_sc_create(&zsc);

  if (zed_err == ZED_ERRORS_NONE) {
    printf("Success\n");
  }
  else {
    printf("Error %s\n", zed_error_str(zed_err));
  }
}