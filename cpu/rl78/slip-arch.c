#include <stdio.h>    /* for putchar(). */

#include "contiki.h"
#include "dev/slip.h"

#include "uart0.h"
#include "slip-arch.h"

/*---------------------------------------------------------------------------*/
void
slip_arch_writeb(unsigned char c)
{
  uart0_putchar(c);
}
/*---------------------------------------------------------------------------*/
/*
 * The serial line is used to transfer IP packets using slip. To make
 * it possible to send debug output over the same line we send debug
 * output as slip frames (i.e delimeted by SLIP_END).
 *
 */
/*---------------------------------------------------------------------------*/
/* #if WITH_UIP */
/* #if (WITH_UIP || WITH_UIP6) */
#if 0
int
putchar(int c)
{
#define SLIP_END 0300
  static char debug_frame = 0;

  if(!debug_frame) {    /* Start of debug output */
    slip_arch_writeb(SLIP_END);
    slip_arch_writeb('\r'); /* Type debug line == '\r' */
    debug_frame = 1;
  }

  slip_arch_writeb((char)c);

  /*
   * Line buffered output, a newline marks the end of debug output and
   * implicitly flushes debug output.
   */
  if(c == '\n') {
    slip_arch_writeb(SLIP_END);
    debug_frame = 0;
  }

  return c;
}
#endif
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port and the SLIP driver.
 *
 */
void
slip_arch_init(unsigned long ubr)
{
  uart1_set_input(slip_input_byte);
}
/*---------------------------------------------------------------------------*/
