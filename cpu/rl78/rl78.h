#ifndef RL78_H
#define RL78_H

#include <stdint.h>

#include "sfrs.h"
#include "sfrs-ext.h"

#define f_CLK         32000000 /* 32 MHz. */
#define CLOCK_CHANNEL 0
#define CLOCK_SCALER  15       /* Use f_CLK / 2^15. */

typedef uint32_t clock_time_t;
typedef unsigned short uip_stats_t;

#endif /* RL78_H */
