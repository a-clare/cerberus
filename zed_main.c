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
  printf("Here\n");
  int32_t sns[3];
  size_t result = 0;
  zed_sc_get_device_list(zsc, sns, 3, &result);

  zed_sc_initialize_sensor(zsc, 29201244);

  printf("Initialized, starting capture\n");
  zed_sc_start(zsc);
}