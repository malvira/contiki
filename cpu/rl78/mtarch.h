#ifndef __MTARCH_H__
#define __MTARCH_H__

#include "contiki-conf.h"

#ifdef MTARCH_CONF_STACKSIZE
#define MTARCH_STACKSIZE MTARCH_CONF_STACKSIZE
#else
#define MTARCH_STACKSIZE 128
#endif

struct mtarch_thread {
  unsigned char stack[MTARCH_STACKSIZE];
  unsigned char *sp;
};

#endif /* __MTARCH_H__ */

