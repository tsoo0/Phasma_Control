// mdsplus_minimal.h
#ifdef __clang__
// Define standard integer types if inttypes.h is not available
#ifndef _STDINT_H
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#endif
#else
// For gcc, use standard headers
#include <stdint.h>
#include <inttypes.h>
#endif

// Then include only the essential MDSplus headers
#include <mdslib.h>
