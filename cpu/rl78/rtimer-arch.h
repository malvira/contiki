#ifndef __RTIMER_ARCH_H__
#define __RTIMER_ARCH_H__

#include "contiki-conf.h"
#include "rl78.h"

#define RTIMER_ARCH_SECOND (15625U)

/* #define rtimer_arch_now() (TCR00) */
#define rtimer_arch_now() (0)

/* void rtimer_isr(void) __interrupt(T1_VECTOR); */

#endif /* __RTIMER_ARCH_H__ */
