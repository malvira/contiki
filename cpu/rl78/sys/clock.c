#include <time.h>

#include "contiki.h"

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

#define clock() (0xffff - TCR[CLOCK_CHANNEL])

void
clock_init(void)
{
#if (CLOCK_CHANNEL <= 7)
  TAU0EN = 1; /* Enable Timer Array Unit 0. */
  TT0 = 0x00ff; /* Stop the Timer Array Unit. */
  TPS0 = (TPS0 & 0xfff0) | CLOCK_SCALER;
  TMR[CLOCK_CHANNEL] = 0x0000; /* default value */

#if (CLOCK_CHANNEL == 0)
  TDR00 = 0xffff;
#elif (CLOCK_CHANNEL == 1)
  TDR01 = 0xffff;
#elif (CLOCK_CHANNEL == 2)
  TDR02 = 0xffff;
#elif (CLOCK_CHANNEL == 3)
  TDR03 = 0xffff;
#elif (CLOCK_CHANNEL == 4)
  TDR04 = 0xffff;
#elif (CLOCK_CHANNEL == 5)
  TDR05 = 0xffff;
#elif (CLOCK_CHANNEL == 6)
  TDR06 = 0xffff;
#elif (CLOCK_CHANNEL == 7)
  TDR07 = 0xffff;
#else
#error Invalid CLOCK_CHANNEL
#endif

  TE0 |= BIT(CLOCK_CHANNEL); /* Start timer channel 0. */
  TS0 |= BIT(CLOCK_CHANNEL); /* Start counting. */
#else
  TAU1EN = 1; /* Enable Timer Array Unit 1. */
  TT1 = 0x00ff; /* Stop the Timer Array Unit. */
  TPS1 = (TPS1 & 0xfff0) | CLOCK_SCALER;
  TMR[CLOCK_CHANNEL] = 0x0000; /* default value */

#if (CLOCK_CHANNEL == 8)
  TDR00 = 0xffff;
#elif (CLOCK_CHANNEL == 9)
  TDR01 = 0xffff;
#elif (CLOCK_CHANNEL == 10)
  TDR02 = 0xffff;
#elif (CLOCK_CHANNEL == 11)
  TDR03 = 0xffff;
#elif (CLOCK_CHANNEL == 12)
  TDR04 = 0xffff;
#elif (CLOCK_CHANNEL == 13)
  TDR05 = 0xffff;
#elif (CLOCK_CHANNEL == 14)
  TDR06 = 0xffff;
#elif (CLOCK_CHANNEL == 15)
  TDR07 = 0xffff;
#else
#error Invalid CLOCK_CHANNEL
#endif

  TE1 |= BIT(CLOCK_CHANNEL); /* Start timer channel. */
  TS1 |= BIT(CLOCK_CHANNEL); /* Start counting. */
#endif
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  return clock();
}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  return clock() / CLOCK_CONF_SECOND;
}
/*---------------------------------------------------------------------------*/

void
clock_wait(clock_time_t t)
{
  clock_time_t t0;
  t0 = clock();
  while(clock() - t0 < t) ;
}
