#ifndef STAMP_H
#define STAMP_H

#include <time.h>

static clock_t _stamp_clock;
#define CLOCKS_PER_MSEC           (CLOCKS_PER_SEC / 1000)
#define CLOCKS_PER_USEC           (CLOCKS_PER_SEC / 1000000)
#define stamp_clock_clear()       do { _stamp_clock = clock(); } while(0)
#define stamp_clock_current_ms()  ((clock() - _stamp_clock) / CLOCKS_PER_MSEC)
#define stamp_clock_print_ms(s)   printf("%s %dms\n", s, (int)stamp_clock_current_ms())
#define stamp_clock_current_us()  ((clock() - _stamp_clock) / CLOCKS_PER_USEC)
#define stamp_clock_print_us(s)   printf("%s %dus\n", s, (int)stamp_clock_current_us())

#endif // STAMP_H
