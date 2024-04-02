#ifndef CERBERUS_ZED_DEFINES_H_
#define CERBERUS_ZED_DEFINES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <time.h>

#if defined _WIN32
#if defined(SL_OC_COMPIL)
#define SL_OC_EXPORT __declspec(dllexport)
#else
#define SL_OC_EXPORT __declspec(dllimport)
#endif
#elif __GNUC__
#define SL_OC_EXPORT __attribute__((visibility("default")))
#if defined(__arm__) || defined(__aarch64__)
#define _SL_JETSON_
#endif
#endif

/*!
 * \brief Get the current system clock as steady clock, so with no jumps even if
 * the system time changes
 * \return the current steady system clock in  nanoseconds
 */
inline uint64_t zed_get_steady_time_stamp() {
  struct timespec ts;
  // Get current time
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

/*!
 * \brief Get the current system clock as wall clock (it can have jumps if the
 * system clock is updated by a sync service) \return the current wall system
 * clock in nanoseconds
 */
inline uint64_t zed_get_wall_time_stamp() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);  // Use CLOCK_REALTIME for wall clock time
  return (uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
}

static const uint16_t ZED_SL_USB_VENDOR               = 0x2b03; //!< Stereolabs Vendor ID
static const uint16_t ZED_SL_USB_PROD_ZED_REVA        = 0xf580; //!< Old ZED firmware Product ID
static const uint16_t ZED_SL_USB_PROD_ZED_M_REVA      = 0xf680;  //!< Old ZED-M binary modified firmware Product ID
static const uint16_t ZED_SL_USB_PROD_ZED_REVB        = 0xf582;  //!< CBS ZED Firmware Product ID
static const uint16_t ZED_SL_USB_PROD_ZED_M_REVB      = 0xf682;  //!< CBS ZED-M Firmware Product ID
static const uint16_t ZED_SL_USB_PROD_ZED_2_REVB      = 0xf780;  //!< CBS ZED 2 Firmware Product ID
static const uint16_t ZED_SL_USB_PROD_ZED_2i          = 0xf880;  //!< CBS ZED 2i Firmware Product ID
static const uint16_t ZED_SL_USB_PROD_MCU_ZEDM_REVA   = 0xf681;  //!< MCU sensor device for ZED-M
static const uint16_t ZED_SL_USB_PROD_MCU_ZED2_REVA   = 0xf781;  //!< MCU sensor device for ZED2
static const uint16_t ZED_SL_USB_PROD_MCU_ZED2i_REVA  = 0xf881;  //!< MCU sensor device for ZED2i

#ifdef __cplusplus
}
#endif
#endif