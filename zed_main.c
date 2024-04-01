#include <hidapi.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

static const uint16_t SL_USB_VENDOR = 0x2b03;
static const uint16_t SL_USB_PROD_MCU_ZED2_REVA = 0xf781; //!< MCU sensor device for ZED2
static const uint16_t SL_USB_PROD_MCU_ZED2i_REVA = 0xf881; //!< MCU sensor device for ZED2i
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

long long current_timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp); // Get current time

    // Convert the time to nanoseconds
    long long ns = (long long)(tp.tv_sec) * 1000000000LL + (long long)(tp.tv_nsec);
    return ns;
}

    // Validity of the magnetometer sensor data
    typedef enum _imu_status {
        NOT_PRESENT = 0,
        OLD_VAL = 1,
        NEW_VAL = 2
    } ImuStatus;

typedef struct Imu
{
    ImuStatus valid;     //!< Indicates if IMU data are valid
    uint64_t timestamp; //!< Timestamp in nanoseconds
    float aX;               //!< Acceleration along X axis in m/s²
    float aY;               //!< Acceleration along Y axis in m/s²
    float aZ;               //!< Acceleration along Z axis in m/s²
    float gX;               //!< Angular velocity around X axis in °/s
    float gY;               //!< Angular velocity around Y axis in °/s
    float gZ;               //!< Angular velocity around > axis in °/s
    float temp;             //!< Sensor temperature in °C
    bool sync;              //!< Indicates in IMU data are synchronized with a video frame
} Imu;

/*!
 * \brief The RAW sensor data structure retrieved from camera MCU by USB
 */
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

int main() {
  struct hid_device_info *devs, *cur_dev;
  if (hid_init() == -1) {
    return 0;
  }

  uint16_t pid = 0;
  wchar_t wsn[256];
  char serial_number[256];
  devs = hid_enumerate(SL_USB_VENDOR, 0x0);
  cur_dev = devs;
  while (cur_dev) {
    int fw_major = cur_dev->release_number >> 8;
    int fw_minor = cur_dev->release_number & 0x00FF;
    pid          = cur_dev->product_id;
    if (!cur_dev->serial_number) {
      cur_dev = cur_dev->next;
      continue;
    }

    printf("Serial number is %ls\n", cur_dev->serial_number);
    printf("Release number %d\n", cur_dev->release_number);
    printf("Current path %s\n", cur_dev->path);
    printf("Manufacturer %ls\n", cur_dev->manufacturer_string);
    printf("Product %ls\n", cur_dev->product_string);
    wcscpy(wsn, cur_dev->serial_number);

    cur_dev = cur_dev->next;
  }
  hid_free_enumeration(devs);

  printf("Serial number is %s\n", serial_number);
  printf("PID %d\n", pid);
  hid_device* dev = hid_open(SL_USB_VENDOR, pid, wsn);

  bool mStopCapture = false;
  bool mGrabRunning = false;

  bool mNewIMUData=false;
  bool mNewMagData=false;
  bool mNewEnvData=false;
  bool mNewCamTempData=false;

  // Read sensor data
  unsigned char usbBuf[65];

  int ping_data_count = 0;

  bool mFirstImuData = true;

  uint64_t rel_mcu_ts = 0;
  uint64_t mMcuTsQueue[50];
  uint64_t mSysTsQueue[50];
  uint64_t mStartSysTs=0;             //!< Initial System Timestamp, to calculate differences [nsec]
  uint64_t mLastMcuTs=0;              //!< MCU Timestamp of the previous data, to calculate relative timestamps [nsec]
  int64_t mSyncOffset=0;
  double mNTPTsScaling=1.0; 
  Imu mLastIMUData;
  while (!mStopCapture) {
    if(ping_data_count>=400) {
      ping_data_count=0;
      // SendPing()
      unsigned char buf[65];
      buf[0] = 0x21;
      buf[1] = 0xF2;
      int res = hid_send_feature_report(dev, buf, 2);
    };
    ping_data_count++;
    mGrabRunning=true;
    usbBuf[1] = 0x01;
    int res = hid_read_timeout( dev, usbBuf, 64, 2000 );
    if( res < (int)(sizeof(RawData)) )  {
      hid_set_nonblocking(dev, 0 );
      continue;
    }

    int target_struct_id = 0;
    if (pid == SL_USB_PROD_MCU_ZED2_REVA || pid==SL_USB_PROD_MCU_ZED2i_REVA)
      target_struct_id = 0x01;

    if( usbBuf[0] != target_struct_id) {
      hid_set_nonblocking( dev, 0 );
      continue;
    }

    RawData* data = (RawData*)usbBuf;
    uint64_t mcu_ts_nsec = (uint64_t)(roundf((float)(data->timestamp)*TS_SCALE));
    if(mFirstImuData && data->imu_not_valid!=1) {
      mStartSysTs = current_timestamp(); // Starting system timestamp

      mLastMcuTs = mcu_ts_nsec;
      mFirstImuData = false;
      continue;
    }
    uint64_t delta_mcu_ts_raw = mcu_ts_nsec - mLastMcuTs;
    mLastMcuTs = mcu_ts_nsec;
    rel_mcu_ts +=  (uint64_t)((double)(delta_mcu_ts_raw)*mNTPTsScaling);
    uint64_t current_data_ts = (mStartSysTs-mSyncOffset) + rel_mcu_ts;

    mLastIMUData.valid = (data->imu_not_valid!=1)?(NEW_VAL):(OLD_VAL);
    mLastIMUData.timestamp = current_data_ts;
    mLastIMUData.aX = data->aX*ACC_SCALE;
    mLastIMUData.aY = data->aY*ACC_SCALE;
    mLastIMUData.aZ = data->aZ*ACC_SCALE;
    mLastIMUData.gX = data->gX*GYRO_SCALE;
    mLastIMUData.gY = data->gY*GYRO_SCALE;
    mLastIMUData.gZ = data->gZ*GYRO_SCALE;
    mLastIMUData.temp = data->imu_temp*TEMP_SCALE;

    printf(" * Timestamp: %lu nsec\n", mLastIMUData.timestamp);
    printf(" * Accelerations [m/s^2] %.3f %3.f %.3f\n", mLastIMUData.aX, mLastIMUData.aY, mLastIMUData.aZ);
  }
}