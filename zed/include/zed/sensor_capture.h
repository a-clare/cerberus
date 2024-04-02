#ifndef CERBERUS_ZED_SENSOR_CAPTURE_H_
#define CERBERUS_ZED_SENSOR_CAPTURE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "zed/defines.h"

typedef enum {
  ZED_IMU_STATUS_NOT_PRESENT = 0,
  ZED_IMU_STATUS_OLD_VAL     = 1,
  ZED_IMU_STATUS_NEW_VAL     = 2
} ZED_IMU_STATUS;

/*!
 * \brief Contains the acquired Imu data
 */
typedef struct {
  ZED_IMU_STATUS valid; //!< Indicates if IMU data are valid
  uint64_t timestamp;   //!< Timestamp in nanoseconds
  float aX;             //!< Acceleration along X axis in m/s²
  float aY;             //!< Acceleration along Y axis in m/s²
  float aZ;             //!< Acceleration along Z axis in m/s²
  float gX;             //!< Angular velocity around X axis in °/s
  float gY;             //!< Angular velocity around Y axis in °/s
  float gZ;             //!< Angular velocity around > axis in °/s
  float temp;           //!< Sensor temperature in °C
  bool sync;            //!< Indicates in IMU data are synchronized with a video frame
} zed_IMU;

// Validity of the magnetometer sensor data
typedef enum {
  ZED_MAG_STATUS_NOT_PRESENT = 0,
  ZED_MAG_STATUS_OLD_VAL     = 1,
  ZED_MAG_STATUS_NEW_VAL     = 2
} ZED_MAG_STATUS;

/*!
 * \brief Contains the acquired Magnetometer data
 */
typedef struct zed_Magnetometer {
  ZED_MAG_STATUS valid;  //!< Indicates if Magnetometer data are valid
  uint64_t timestamp;    //!< Timestamp in nanoseconds
  float mX;              //!< Acceleration along X axis in uT
  float mY;              //!< Acceleration along Y axis in uT
  float mZ;              //!< Acceleration along Z axis in uT
} zed_Magnetometer;


// Validity of the environmental sensor data
typedef enum {
  ZED_ENV_STATUS_NOT_PRESENT = 0,
  ZED_ENV_STATUS_OLD_VAL     = 1,
  ZED_ENV_STATUS_NEW_VAL     = 2
} ZED_ENV_STATUS;

/*!
 * \brief Contains the acquired Environment data
 */
typedef struct {
  ZED_ENV_STATUS valid; //!< Indicates if Environmental data are valid
  uint64_t timestamp;   //!< Timestamp in nanoseconds
  float temp;           //!< Sensor temperature in °C
  float press;          //!< Atmospheric pressure in hPa
  float humid;          //!< Humidity in %rH
} zed_Environment;

typedef enum {
  ZED_TEMP_STATUS_NOT_PRESENT = 0,
  ZED_TEMP_STATUS_OLD_VAL     = 1,
 ZED_TEMP_STATUS_NEW_VAL      = 2
} ZED_TEMP_STATUS;

/*!
 * \brief Contains the acquired Camera Temperature data
 */
typedef struct {
  ZED_TEMP_STATUS valid;  //!< Indicates if camera temperature data are valid
  uint64_t timestamp;     //!< Timestamp in nanoseconds
  float temp_left;        //!< Temperature of the left CMOS camera sensor
  float temp_right;       //!< Temperature of the right CMOS camera sensor
} zed_Temperature;

// "zed_SensorCapture" typically abreviated to zsc
typedef struct zed_SensorCapture zed_SensorCapture;

zed_SensorCapture* zsc_create();

void zsc_destroy(zed_SensorCapture** zsc);

/**
 * @brief Gets the serial numbers for all ZED devices.
 * DOES NOT allocate serialNumbers, expects caller to have an int[] allocated
 * 
 * @param zsc sensor capture object working with
 * @param serialNumbers the array that serial numbers will be written into
 * @param maxSN max number of elements in serialNumbers[]
 * @return size_t the number of serial numbers found || maxSN if reached limit
 */
size_t zsc_get_device_list(zed_SensorCapture* zsc,
                           int32_t* serialNumbers,
                           size_t maxSN);

/**
 * @brief Initialize a connection to a ZED sensor specified by serial number sn
 * 
 * @param zsc sensor capture object working with
 * @param sn the serial number of the device wanting to connect to
 * @return true if successfully initialized
 * @return false otherwise
 */
bool zsc_initialize_sensor(zed_SensorCapture* zsc,
                           int32_t sn);

/**
 * @param timeout how long to wait for grabbing imu data before returning
 * @return const zed_IMU* the last received IMU data
 * @return NULL if something went wrong
 */
const zed_IMU* zsc_get_last_imu(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing magnetometer data before returning
 * @return const zed_Magnetometer* the last received magnetometer data
 * @return NULL if something went wrong
 */
const zed_Magnetometer* zsc_get_last_mag(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing environment data before returning
 * @return const zed_Environment* the last received environment data
 * @return NULL if something went wrong
 */
const zed_Environment* zsc_get_last_env(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing camera temperature sensor data before returning
 * @return const zed_Temperature* the last received tempertature data
 * @return NULL if something went wrong
 */
const zed_Temperature* zsc_get_last_temperature(uint64_t timeout);

/**
 * @brief Perform a SW reset of the sensor with serial number 'sn'
 * 
 * @param sn serial number of the sensor being reset
 * @return true if successful
 * @return false otherwise
 */
bool zsc_reset_sensor(int32_t sn);  
  

#ifdef __cplusplus
}
#endif
#endif