#include "zed/sensor_capture.h"
#include "hidapi.h"
#include <stdlib.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>

static const uint16_t ZED_SL_USB_VENDOR = 0x2b03; //!< Stereolabs Vendor ID

struct zed_SensorCapture {
  // The handle for working with a HID device (the ZED camera over USB)
  hid_device* hid_handle;
  int32_t serial_number;
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
  if (hid_init() == -1) {
    printf("hid_init failed\n");
    return ZED_ERRORS_CONNECT_FAIL;
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

    // Increment our return count of how many zed sensors we found
    *numSN += 1;
    if (*numSN >= maxSN) {
      break;
    }
    cur_dev = cur_dev->next;
  }

  hid_free_enumeration(devs);
  return ZED_ERRORS_NONE;
}

ZED_ERRORS zed_sc_initialize_sensor(zed_SensorCapture* zsc,
                                    int32_t sn) {
  if (zsc == NULL) {
    return ZED_ERRORS_NULL_INPUT;
  }
  // Reset our internal parameters on new initialize
  zsc->hid_handle    = NULL;
  zsc->serial_number = -1;

  struct hid_device_info *devs, *cur_dev;
  if (hid_init() == -1) {
    printf("hid_init failed\n");
    return ZED_ERRORS_CONNECT_FAIL;
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

    int cur_dev_sn = wcstol(cur_dev->serial_number, NULL, 10);
    if (cur_dev_sn == sn) {
      printf("Found serial number %d\n", sn);
      zsc->hid_handle = hid_open(ZED_SL_USB_VENDOR, pid, cur_dev->serial_number);
      if (zsc->hid_handle == NULL) {
        printf("Error opening hid device\n");
        return ZED_ERRORS_CONNECT_FAIL;
      }
      zsc->serial_number = cur_dev_sn;
      break;
    }

    cur_dev = cur_dev->next;
  }
  hid_free_enumeration(devs);
  // If the handle is still NULL its likely there is no ZED device connected, or we have the wrong
  // serial number input
  if (zsc->hid_handle == NULL) {
    return ZED_ERRORS_CONNECT_FAIL;
  }
  return ZED_ERRORS_NONE;
}

static const uint8_t REP_ID_SENSOR_DATA = 0x01;
static const uint8_t REP_ID_REQUEST_SET = 0x21;
static const uint8_t RQ_CMD_PING = 0xF2;

static void send_ping(zed_SensorCapture* zsc) {
  printf("Sending ping\n");
  unsigned char buf[65];
  buf[0] = REP_ID_REQUEST_SET;
  buf[1] = RQ_CMD_PING;

  int res = hid_send_feature_report(zsc->hid_handle, buf, 2);
  if (res < 0) {
    printf("Unable to send ping\n");
  }
  printf("Sent ping\n");
}
#pragma pack(push)  // push current alignment to stack
#pragma pack(1)     // set alignment to 1 byte boundary
typedef struct RawData {
    uint8_t struct_id;		//!< Struct identifier, it matches the USB HID Report ID
    uint8_t imu_not_valid; 	//!< Indicate if IMU data are valid [0->valid, 1->not_valid]
    uint64_t timestamp;		//!< Data timestamp (from IMU sensor) [usec/39]
    int16_t gX;				//!< Raw Gyroscope X
    int16_t gY;				//!< Raw Gyroscope Y
    int16_t gZ;				//!< Raw Gyroscope Z
    int16_t aX;				//!< Raw Accelerometer X
    int16_t aY;				//!< Raw Accelerometer Y
    int16_t aZ;				//!< Raw Accelerometer Z
    uint8_t frame_sync;		//!< Indicates if data are synced to a camera frame
    uint8_t sync_capabilities; //!< Indicates if frame synchronization is active
    uint32_t frame_sync_count; //!< Counts the number of synced frames
    int16_t imu_temp;		//!< Temperature from the IMU sensor [0.01 °C]
    uint8_t mag_valid;		//!< Indicates if Magnetometer data are valid (put to 0 if no magnetometer is present)
    int16_t mX;				//!< Raw Magnetometer X
    int16_t mY;				//!< Raw Magnetometer Y
    int16_t mZ;				//!< Raw Magnetometer Z
    uint8_t camera_moving;	//!< Indicate if the camera is moving (uses BMI internal HW)
    uint32_t camera_moving_count; //!< Counts the number of camera moving interrupts
    uint8_t camera_falling;	//!< Indicate if the camera is free falling (uses BMI internal HW)
    uint32_t camera_falling_count; //!< Counts the number of camera falling interrupts
    uint8_t env_valid;		//!< Indicate if Environmental data are valid (put to `ENV_SENS_NOT_PRESENT` if no environmental sensor is present)
    int16_t temp;			//!< Temperature [0.01 °C]
    uint32_t press;			//!< Pressure [0.01 hPa]
    uint32_t humid;			//!< Relative humidity [1.0/1024.0 %rH]
    int16_t temp_cam_left;	//!< Temperature of the left camera sensor [0.01 °C]
    int16_t temp_cam_right; //!< Temperature of the right camera sensor [0.01 °C]
} RawData;

#define TS_SCALE        (39062.5f)
#define DEFAULT_GRAVITY (9.8189f)
#define ACC_SCALE       (DEFAULT_GRAVITY*(8.0f/32768.0f))
#define GYRO_SCALE      (1000.0f/32768.0f)
#define MAG_SCALE       (1.0f/16.0f)
#define TEMP_SCALE      (0.01f)
#define PRESS_SCALE_NEW (0.0001f)       // FM >= V3.9
#define PRESS_SCALE_OLD (0.01f)         // FW < v3.9
#define HUMID_SCALE_NEW (0.01f)         // FM >= V3.9
#define HUMID_SCALE_OLD (1.0f/1024.0f)  // FW < v3.9

void zed_sc_start(zed_SensorCapture* zsc) {

  // enableDataStream()
  unsigned char buf[65];
  buf[0] = 0x32;
  buf[1] = true ? 1 : 0;

  int res = hid_send_feature_report(zsc->hid_handle, buf, 2);
  if (res < 0) {
    return;
  }
    // Read sensor data
    unsigned char usbBuf[65] = {0};

    int ping_data_count = 0;

    bool mFirstImuData = true;

    uint64_t rel_mcu_ts = 0;
    uint64_t mMcuTsQueue[50] = {0};
    uint64_t mSysTsQueue[50] = {0};
    uint64_t mStartSysTs = 0;
    uint64_t mLastMcuTs = 0;
    int64_t mSyncOffset = 0;
    double mNTPTsScaling = 1.0;
    for (;;) {
      // sending a ping aboutonce per second
      // to keep the streaming alive
      if (ping_data_count >= 400) {
        ping_data_count = 0;
        send_ping(zsc);
      };
      ping_data_count++;

      usbBuf[1] = REP_ID_SENSOR_DATA;
      int res = hid_read_timeout(zsc->hid_handle, usbBuf, 64, 2000);
      if (res < (int)(sizeof(RawData))) {
        hid_set_nonblocking(zsc->hid_handle, 0);
        continue;
      }

      RawData* data = (RawData*)usbBuf;

      uint64_t mcu_ts_nsec =
          (int64_t)(roundf((float)(data->timestamp) * TS_SCALE));

      if (mFirstImuData && data->imu_not_valid != 1) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME,
                      &ts);  // Use CLOCK_REALTIME for wall clock time
        mStartSysTs = (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
        // std::cout << "SensorCapture: " << mStartSysTs << std::endl;

        mLastMcuTs = mcu_ts_nsec;
        mFirstImuData = false;
        continue;
      }

      uint64_t delta_mcu_ts_raw = mcu_ts_nsec - mLastMcuTs;
      mLastMcuTs = mcu_ts_nsec;
      rel_mcu_ts += (uint64_t)((double)(delta_mcu_ts_raw)*mNTPTsScaling);

      // mStartSysTs is synchronized to Video TS when sync is enabled using \ref
      // VideoCapture::enableSensorSync
      uint64_t current_data_ts = (mStartSysTs - mSyncOffset) + rel_mcu_ts;

      printf("Got IMU data %lu %.3f %.3f %.3f %.3f %.3f %.3f\n",
             current_data_ts, data->aX * ACC_SCALE, data->aY * ACC_SCALE,
             data->aZ * ACC_SCALE, data->gX * GYRO_SCALE, data->gY * GYRO_SCALE,
             data->gZ * GYRO_SCALE);
    }
  }