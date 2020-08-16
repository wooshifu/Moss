#pragma once

void uart0_init();
void uart0_send(unsigned int c);
char uart0_getc();
void uart0_puts(char *s);
void uart_hex(unsigned int d);
