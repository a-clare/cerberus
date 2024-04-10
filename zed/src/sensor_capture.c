#include "zed/sensor_capture.h"
#include "hidapi.h"
#include <stdlib.h>
#include <stdio.h>

static const uint16_t ZED_SL_USB_VENDOR = 0x2b03; //!< Stereolabs Vendor ID

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
  if (zsc == NULL || (*zsc) == NULL) {
    return ZED_ERRORS_NULL_INPUT;
  }
  // Free the memory and make the input pointer NULL in case someone tries
  // to use it again after destroyed
  free(*zsc);
  *zsc = NULL;
  return ZED_ERRORS_NONE;
}

ZED_ERRORS zed_sc_get_device_list(zed_SensorCapture* zsc,
                                  int32_t* serialNumbers,
                                  size_t maxSN,
                                  size_t* numSN) {
  if (zsc == NULL) {
    return ZED_ERRORS_NULL_INPUT;
  }

  struct hid_device_info *devs, *cur_dev;
  // TODO: What to actually return here, a ZED_ERROR of some form
  if (hid_init() == -1) {
    printf("hud_init failed\n");
    return 0;
  }
  devs = hid_enumerate(ZED_SL_USB_VENDOR, 0x0);
  cur_dev = devs;
  while (cur_dev) {
    int fw_major = cur_dev->release_number >> 8;
    int fw_minor = cur_dev->release_number & 0x00FF;
    uint16_t pid = cur_dev->product_id;
    if (!cur_dev->serial_number) {
      cur_dev = cur_dev->next;
      continue;
    }
    printf("ZED Device Found\n");
    printf("VID: %X PID: %X\n ", cur_dev->vendor_id, cur_dev->product_id);
    printf("Path: %s\n", cur_dev->path);
    printf("Serial Number: %ls\n", cur_dev->serial_number);
    printf("Manufacturer: %ls\n", cur_dev->manufacturer_string);
    printf("Product: %ls\n", cur_dev->product_string);
    printf("Release number: %d.%d\n", fw_major, fw_minor);
    cur_dev = cur_dev->next;
  }

  hid_free_enumeration(devs);
}