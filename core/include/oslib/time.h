#ifndef TIME
#define TIME

#include <stdint.h>

typedef uint64_t time_t;

struct tm {
    uint8_t tm_sec;
    uint8_t tm_min;
    uint8_t tm_hour;
    uint8_t tm_mday;
    uint8_t tm_mon;
    uint16_t tm_year;
    uint8_t tm_wday;
    uint16_t tm_yday;
    uint8_t tm_isdst;
};

/*
tm_sec	int	seconds after the minute	0-60*
tm_min	int	minutes after the hour	0-59
tm_hour	int	hours since midnight	0-23
tm_mday	int	day of the month	1-31
tm_mon	int	months since January	0-11
tm_year	int	years since 1900
tm_wday	int	days since Sunday	0-6
tm_yday	int	days since January 1	0-365
tm_isdst	int	Daylight Saving Time flag
*/

struct tm * localtime(time_t * t);
struct tm * gmtime(time_t * t);
time_t time(time_t * sec);
time_t mktime (struct tm * timeptr);

#endif // TIME

