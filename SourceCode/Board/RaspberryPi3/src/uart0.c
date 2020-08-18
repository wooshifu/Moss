#include "gpio.h"
#include "log.h"
#include "mailbox.h"

/* PL011 UART registers */
#define UART0_DATA_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x00201000))
#define UART0_FLAG_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x00201018))
#define UART0_INTEGER_BAUD_RATE_DIVISOR ((volatile unsigned int *)(MMIO_BASE + 0x00201024))
#define UART0_FRACTIONAL_BAUD_RATE_DIVISOR ((volatile unsigned int *)(MMIO_BASE + 0x00201028))
#define UART0_LINE_CONTROL_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x0020102C))
#define UART0_CONTROL_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x00201030))
#define UART0_INTERRUPT_MASK_SET_CLEAR_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x00201038))
#define UART0_INTERRUPT_CLEAR_REGISTER ((volatile unsigned int *)(MMIO_BASE + 0x00201044))

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void init_uart0() {
  /* initialize UART */
  *UART0_CONTROL_REGISTER = 0; // turn off UART0

  mailbox_property_set_clock_rate_t data = {.size = sizeof(struct mailbox_property_set_clock_rate_t),
                                            .code = MAILBOX_CODE_REQUEST,

                                            .tag = MAILBOX_PROPERTY_TAG_SET_CLOCK_RATE,
                                            .buffer_size = 12, // 12 bytes
                                            .tag_code = MAILBOX_CODE_REQUEST,
                                            .clock_id = MAILBOX_CLOCK_ID_UART,
                                            .rate = 4000000,
                                            .skip_setting_turbo = 0,

                                            .end = MAILBOX_PROPERTY_TAG_END};
  mailbox_call(MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VIDEO_CORE, &data);
  log_d("response code: %x, tag code: %x", data.code, data.tag_code);
  if (!is_valid_mailbox_response(data.code, data.tag_code)) {
    // NOTE: uart0 not initialized, could we log this message to uart0???
    log_f("failed to set clock uart rate, code: %u, tag_code: %u", data.code, data.tag_code);
    return;
  }
  log_d("uart0(id:%u) clock rate initialized to rate:%u", data.clock_id, data.rate);

  /* map UART0 to GPIO pins */
  unsigned int r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15, [17:15]:FSEL15, [14:12]:FSEL14
  r |= (4 << 12) | (4 << 15);    // alt0, 0b100 = GPIO Pin takes alternate function 0
  *GPFSEL1 = r;
  *GPPUD = 0; // enable pins 14 and 15
  r = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = 0;                            // flush GPIO setup
  *UART0_INTERRUPT_CLEAR_REGISTER = 0x7FF;   // clear interrupts
  *UART0_INTEGER_BAUD_RATE_DIVISOR = 2;      // 115200 baud, 4000000/(16*115200)
  *UART0_FRACTIONAL_BAUD_RATE_DIVISOR = 0xB; // BAUDDIV = (FUARTCLK/(16 * Baud rate))
  *UART0_LINE_CONTROL_REGISTER = 0b11 << 5;  // 8n1, b11 = 8 bits
  *UART0_CONTROL_REGISTER = 0x301;           // enable Tx, Rx, FIFO
  log_d("uart0 successfully initialized");
}

/**
 * Send a character
 */
void uart0_send(unsigned int character) {
  /* wait until we can send */
  do {
    asm volatile("nop");
  } while (*UART0_FLAG_REGISTER & 0x20);
  /* write the character to the buffer */
  *UART0_DATA_REGISTER = character;
}

/**
 * Receive a character
 */
char uart0_getc() {
  /* wait until something is in the buffer */
  do {
    asm volatile("nop");
  } while (*UART0_FLAG_REGISTER & 0x10);
  /* read it and return */
  char r = (char)(*UART0_DATA_REGISTER);
  /* convert carriage return to newline */
  return r == '\r' ? '\n' : r;
}

/**
 * Display a string
 */
void uart0_puts(char *string) {
  while (*string) {
    /* convert newline to carriage return + newline */
    if (*string == '\n') {
      uart0_send('\r');
    }
    uart0_send(*string++);
  }
}

/**
 * hal put_char implementation
 * @param character the character
 */
void put_char(char character) { uart0_send(character); }
