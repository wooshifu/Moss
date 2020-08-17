#include "gpio.h"
#include "log.h"
#include "mbox.h"

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

  /*
   */
  /* set up clock for consistent divisor values */ /*

     mailbox[0] = 9 * 4;
     mailbox[1] = MAILBOX_REQUEST;
     mailbox[2] = MAILBOX_TAG_SET_CLOCK_RATE; // set clock rate
     mailbox[3] = 12;                         // value buffer size in bytes
     mailbox[4] = 8;                          // request code
     mailbox[5] = 2;                          // UART clock, clock id
     mailbox[6] = 4000000;                    // 4Mhz, rate (in Hz)
     mailbox[7] = 0;                          // clear turbo, setting turbo
     mailbox[8] = MAILBOX_TAG_LAST;
     int result = mailbox_call(MAILBOX_CHANNEL_PROPERTY_ARM_TO_VIDEO_CORE);
   */
                                                   /*  property_set_clock_rate_t set_clock_rate = {
                                                         .tag = PROPERTY_TAG_SET_CLOCK_RATE,
                                                         .code = MAILBOX_CODE_REQUEST,
                                                         .buffer_size = 12,
                                                         .clock_id = CLOCK_ID_UART,
                                                         .rate = 4000000,
                                                         .skip_setting_turbo = 0,
                                                     };*/
  log_d("sizeof(property_set_clock_rate_mail_t):%d", sizeof(property_set_clock_rate_mail_t));
  property_set_clock_rate_mail_t set_clock_rate_mail = {
      .size = sizeof(property_set_clock_rate_mail_t),
      .code = MAILBOX_CODE_REQUEST,
      .property =
          {
              .tag = PROPERTY_TAG_SET_CLOCK_RATE,
              .buffer_size = 12, // todo: should be 32 aligned
              .code = MAILBOX_CODE_REQUEST,
              .clock_id = CLOCK_ID_UART,
              .rate = 4000000,
              .skip_setting_turbo = 0,
          },
      .end = PROPERTY_TAG_END,
  };
  //  log_d("addr: %p", &set_clock_rate);
  mailbox_message_t message = {.channel = MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VIDEO_CORE,
                               .data = CAST_TO_MAILBOX_DATA(set_clock_rate_mail)};
  /*mailbox_message_t response =*/mailbox_send(&message);
  if (set_clock_rate_mail.code == MAILBOX_CODE_RESPONSE_FAILURE ||
      set_clock_rate_mail.property.code == MAILBOX_CODE_RESPONSE_FAILURE) {
    log_e("could not set mailbox, error code: %d, property code: %d", set_clock_rate_mail.code,
          set_clock_rate_mail.property.code);
    return;
  }

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
