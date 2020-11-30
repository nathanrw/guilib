#ifndef NUI_TYPES_H
#define NUI_TYPES_H

#include "stdint.h"

#define NUI_API /* TODO: exports */

/* Integer types */
typedef uint32_t nui_unsigned;
typedef int32_t nui_int;
typedef uint64_t nui_size;
typedef float nui_real;
typedef bool nui_bool;
typedef unsigned char nui_byte;

/* Strings are utf-8 */
typedef const char* nui_utf8;

/* Widget identifier. 0 always identifies the root widget, which always exists
   and cannot be destroyed. */
typedef uint32_t nui_widget_id;

enum nui_widget_type {
  NUI_WIDGET_TYPE_WINDOW = 0,
  NUI_WIDGET_TYPE_BUTTON
};

enum nui_error_code {
  NUI_ERROR_OK = 0,
  NUI_ERROR_UNKNOWN,
  NUI_ERROR_BUFFER_TOO_SMALL,
  NUI_ERROR_ENCODING
};

#endif
