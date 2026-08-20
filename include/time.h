#ifndef _MCPP_NOLIBC_TIME_H
#define _MCPP_NOLIBC_TIME_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef long time_t;
typedef long clock_t;
struct timespec { time_t tv_sec; long tv_nsec; };
struct tm { int tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year,
                tm_wday, tm_yday, tm_isdst; };
#ifdef __cplusplus
}
#endif
#endif
