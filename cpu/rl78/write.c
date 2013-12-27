
#include <stddef.h> /* for size_t. */

#include "uart0.h"
#include "write.h"

int
write(int fd, const void *buf, size_t count)
{
  size_t n;
  for(n = 0; n < count; n++) {
    uart0_putchar(((const char *)buf)[n]);
  }
  return count;
}
#ifdef __IAR_SYSTEMS_ICC__
size_t
__write(int fd, const unsigned char *buf, size_t count)
{
  write(fd, buf, count);
}
#endif
