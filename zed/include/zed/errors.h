#ifndef CERBERUS_ZED_ERRORS_H_
#define CERBERUS_ZED_ERRORS_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  // Return on success, or no errors
  ZED_ERRORS_NONE,
  // Return if a memory allocation failed
  ZED_ERRORS_ALLOC_FAIL,
  // Return when a function receives a NULL input
  ZED_ERRORS_NULL_INPUT,
} ZED_ERRORS;

const char* zed_error_str(ZED_ERRORS err);

#ifdef __cplusplus
}
#endif
#endif