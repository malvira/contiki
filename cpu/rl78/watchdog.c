#include "rl78.h"
#include "watchdog.h"

void
watchdog_periodic(void)
{
  WDTE = 0xAC;
}
