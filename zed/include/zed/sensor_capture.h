/**
 * @file sensor_capture.h
 * @brief Contains functionality for reading non camera sensor data from a ZED 2 camera
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef CERBERUS_ZED_SENSOR_CAPTURE_H_
#define CERBERUS_ZED_SENSOR_CAPTURE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "zed/defines.h"
#include "zed/types.h"
#include "zed/errors.h"

// "zed_SensorCapture" typically abreviated to zed_sc, or zsc
typedef struct zed_SensorCapture zed_SensorCapture;

/**
 * @brief Create a new sensor capture object.
 * 
 * Allocates memory, so when done with the zsc you need to call zed_sc_destroy
 * 
 * @param zsc pointer (to pointer) to zsc object
 * @return ZED_ERRORS_NONE if all goes well
 * @return ZED_ERRORS_ALLOC_FAIL if a memory allocation error occurred 
 * @return ZED_ERRORS_NULL_INPUT if the input pointer is null
 */
ZED_ERRORS zed_sc_create(zed_SensorCapture** zsc);

/**
 * @brief Destroy object (free memory allocated)
 * 
 * @param zsc 
 * @return ZED_ERRORS_NONE if all goes well
 * @return ZED_ERRORS_NULL_INPUT if the input pointer is null
 */
ZED_ERRORS zed_sc_destroy(zed_SensorCapture** zsc);

/**
 * @brief Gets the serial numbers for all ZED devices.
 * DOES NOT allocate serialNumbers, expects caller to have an int[] allocated
 * 
 * @param zsc sensor capture object working with
 * @param serialNumbers the array that serial numbers will be written into
 * @param maxSN max number of elements in serialNumbers[]
 * @return size_t the number of serial numbers found || maxSN if reached limit
 */
size_t zed_sc_get_device_list(zed_SensorCapture* zsc,
                              int32_t* serialNumbers,
                              size_t maxSN);

/**
 * @brief Initialize a connection to a ZED sensor specified by serial number sn
 * 
 * @param zsc sensor capture object working with
 * @param sn the serial number of the device wanting to connect to
 */
ZED_ERRORS zed_sc_initialize_sensor(zed_SensorCapture* zsc,
                                    int32_t sn);

/**
 * @param timeout how long to wait for grabbing imu data before returning
 * @return const zed_IMU* the last received IMU data
 * @return NULL if something went wrong
 */
const zed_IMU* zed_sc_get_last_imu(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing magnetometer data before returning
 * @return const zed_Magnetometer* the last received magnetometer data
 * @return NULL if something went wrong
 */
const zed_Magnetometer* zed_sc_get_last_mag(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing environment data before returning
 * @return const zed_Environment* the last received environment data
 * @return NULL if something went wrong
 */
const zed_Environment* zed_sc_get_last_env(uint64_t timeout);

/**
 * @param timeout how long to wait for grabbing camera temperature sensor data before returning
 * @return const zed_Temperature* the last received tempertature data
 * @return NULL if something went wrong
 */
const zed_Temperature* zed_sc_get_last_temperature(uint64_t timeout);
 
#ifdef __cplusplus
}
#endif
#endif