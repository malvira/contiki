#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

/* mc1322x files */
#include "contiki-mc1322x-conf.h"

#define PLATFORM_HAS_LEDS    0
#define PLATFORM_HAS_BUTTON  1

/* Econotag I tune parameters */
#define ECONOTAG_CTUNE_4PF 1
/* Coarse tune: add 0-15 pf (CTUNE is 4 bits) */
#define ECONOTAG_CTUNE 11
/* Fine tune: add FTUNE * 156fF (FTUNE is 5bits) */
#define ECONOTAG_FTUNE 7

/* M12 tune parameters */
#define M12_CTUNE_4PF 1
#define M12_CTUNE 3
#define M12_FTUNE 3

/* the econotag platform will correctly detect an Econotag I (no M12) vs. Econotag II (w/M12) */
/* and trim the main crystal accordingly */
/* this detection will be incorrect if you populate the 32.768kHz crystal on the Econotag I */
/* In that case, you should FORCE_ECONOTAG_I below */
#define FORCE_ECONOTAG_I 0

/* if you define a serial number then it will be used to comput the mac address */
/* otherwise, a random mac address in the Redwire development IAB will be used */
/* #define M12_CONF_SERIAL 0x000000 */

/* Clock ticks per second */
#define CLOCK_CONF_SECOND 100

#define CCIF
#define CLIF

#define CONSOLE_UART UART1
#define CONSOLE_BAUD 115200

#define dbg_putchar(x) uart1_putc(x)

#define USE_FORMATTED_STDIO         1
#define MACA_DEBUG                  0
#define CONTIKI_MACA_RAW_MODE       0

#define NETSTACK_CONF_RADIO   contiki_maca_driver
/* wait for the maca to tx the packet */
/* currently you must enable BLOCKING_TX to get proper status back from AUTOACKs */
/* i.e. BLOCKING_TX is necessary for csma_driver to work */
#define BLOCKING_TX 1
#define MACA_AUTOACK 1
#define NULLRDC_CONF_802154_AUTOACK_HW 1

#define USE_WDT 0

#ifndef WDT_TIMEOUT
#define WDT_TIMEOUT 5000 /* watchdog timeout in ms */
#endif

/* Core rtimer.h defaults to 16 bit timer unless RTIMER_CLOCK_LT is defined */
typedef unsigned long rtimer_clock_t;
#define RTIMER_CLOCK_LT(a,b)     ((signed long)((a)-(b)) < 0)

/* end of mc1322x specific config. */

#endif
