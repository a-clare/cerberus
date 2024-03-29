#include <hidapi.h>
#include <stdint.h>
#include <stdio.h>

static const uint16_t SL_USB_VENDOR = 0x2b03;

int main() {
  struct hid_device_info *devs, *cur_dev;
  if (hid_init() == -1) {
    return 0;
  }

  devs = hid_enumerate(SL_USB_VENDOR, 0x0);
  cur_dev = devs;
  while (cur_dev) {
    int fw_major = cur_dev->release_number >> 8;
    int fw_minor = cur_dev->release_number & 0x00FF;
    uint16_t pid = cur_dev->product_id;
    if (!cur_dev->serial_number) {
      cur_dev = cur_dev->next;
      continue;
    }

    printf("Serial number is %ls\n", cur_dev->serial_number);
    cur_dev = cur_dev->next;
  }
}