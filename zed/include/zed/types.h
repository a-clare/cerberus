#ifndef CERBERUS_ZED_TYPES_H_
#define CERBERUS_ZED_TYPES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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

#ifdef __cplusplus
}
#endif
#endif