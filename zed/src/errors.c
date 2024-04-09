#include "zed/errors.h"

const char* zed_error_str(ZED_ERRORS err) {
  switch(err) {
    case ZED_ERRORS_NONE: 
      return "ZED ERROR: No Error";
    case ZED_ERRORS_ALLOC_FAIL: 
      return "ZED ERROR: Memory allocation failed";
    case ZED_ERRORS_NULL_INPUT:
      return "ZED ERROR: NULL input";
    default: 
      return "ZED ERROR: Unknown error";
  }
}