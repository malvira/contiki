#ifndef UART0_H__
#define UART0_H__

void uart0_init(void);
void uart0_putchar(int c);
#define uart0_can_getchar() (SRIF0)
char uart0_getchar(void);

int uart0_puts(const char *s);

#endif /* UART0_H__ */
