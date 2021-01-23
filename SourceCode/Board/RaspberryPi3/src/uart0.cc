#include "hal/stdio.hh"
#include "libcxx/log.hh"
#include "rpi3/mailbox.hh"
#include "rpi3/mmio.hh"
#include "rpi3/uart0.hh"

/* PL011 UART registers */
#define UART0_DATA_REGISTER                     ((volatile unsigned int*)(MMIO_BASE + 0x00201000))
#define UART0_FLAG_REGISTER                     ((volatile unsigned int*)(MMIO_BASE + 0x00201018))
#define UART0_INTEGER_BAUD_RATE_DIVISOR         ((volatile unsigned int*)(MMIO_BASE + 0x00201024))
#define UART0_FRACTIONAL_BAUD_RATE_DIVISOR      ((volatile unsigned int*)(MMIO_BASE + 0x00201028))
#define UART0_LINE_CONTROL_REGISTER             ((volatile unsigned int*)(MMIO_BASE + 0x0020102C))
#define UART0_CONTROL_REGISTER                  ((volatile unsigned int*)(MMIO_BASE + 0x00201030))
#define UART0_INTERRUPT_MASK_SET_CLEAR_REGISTER ((volatile unsigned int*)(MMIO_BASE + 0x00201038))
#define UART0_INTERRUPT_CLEAR_REGISTER          ((volatile unsigned int*)(MMIO_BASE + 0x00201044))

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void init_uart0() {
  /* initialize UART */
  *UART0_CONTROL_REGISTER = 0; // turn off UART0

  mailbox_property_set_clock_rate_t data = {.size = sizeof(mailbox_property_set_clock_rate_t),
                                            .code = MAILBOX_CODE_REQUEST,

                                            .tag                = MAILBOX_PROPERTY_TAG_SET_CLOCK_RATE,
                                            .buffer_size        = 12, // 12 bytes, clock_id,rate,skip_setting_turbo
                                            .tag_code           = MAILBOX_CODE_REQUEST,
                                            .clock_id           = MAILBOX_CLOCK_ID_UART,
                                            .rate               = 4000000,
                                            .skip_setting_turbo = 0,

                                            .end = MAILBOX_PROPERTY_TAG_END};
  mailbox_call(MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VIDEO_CORE, &data);
  //  log_d("response code: %x, tag code: %x", data.code, data.tag_code);
  if (!is_valid_mailbox_response(data.code, data.tag_code)) {
    // NOTE: uart0 not initialized, could we log this message to uart0???
    log_f("failed to set clock uart rate, code: %u, tag_code: %u", data.code, data.tag_code);
    return;
  }
  //  log_d("uart0(id:%u) clock rate initialized to rate:%u", data.clock_id, data.rate);

  /* map UART0 to GPIO pins */
  unsigned int r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15, [17:15]:FSEL15, [14:12]:FSEL14
  r |= (4 << 12) | (4 << 15);    // alt0, 0b100 = GPIO Pin takes alternate function 0
  *GPFSEL1 = r;
  *GPPUD   = 0; // enable pins 14 and 15
  r        = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r          = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0                          = 0;         // flush GPIO setup
  *UART0_INTERRUPT_CLEAR_REGISTER     = 0x7FF;     // clear interrupts
  *UART0_INTEGER_BAUD_RATE_DIVISOR    = 2;         // 115200 baud, 4000000/(16*115200)
  *UART0_FRACTIONAL_BAUD_RATE_DIVISOR = 0xB;       // BAUDDIV = (FUARTCLK/(16 * Baud rate))
  *UART0_LINE_CONTROL_REGISTER        = 0b11 << 5; // 8n1, b11 = 8 bits
  *UART0_CONTROL_REGISTER             = 0x301;     // enable Tx, Rx, FIFO
  //  log_d("uart0 successfully initialized");
}

/**
 * Send a character
 */
int uart0_send(int character) {
  /* wait until we can send */
  do {
    asm volatile("nop");
  } while (*UART0_FLAG_REGISTER & 0x20);
  /* write the character to the buffer */
  *UART0_DATA_REGISTER = character;
  return character;
}

/**
 * Receive a character
 */
char uart0_getc() {
  /* wait until something is in the buffer */
  // todo: don't use while loop, use uart0 triggered event
  do {
    asm volatile("nop");
  } while (*UART0_FLAG_REGISTER & 0x10);
  /* read it and return */
  char r = (char)(*UART0_DATA_REGISTER);
  /* convert carriage return to newline */
  return r /*== '\r' ? '\n' : r*/;
}

/**
 * Display a string
 */
void uart0_puts(char* string) {
  while (*string) {
    /* convert newline to carriage return + newline */
    if (*string == '\n') {
      uart0_send('\r');
    }
    uart0_send(*string++);
  }
}

void uart_send(unsigned int c);

/**
 * hal putchar implementation
 * @param character the character
 */
int putchar(int character) {
  uart_send(character);
  return 0;
}

int getchar() { return uart0_getc(); }

/* PL011 UART registers */
#define UART0_DR   ((volatile unsigned int*)(MMIO_BASE + 0x00201000))
#define UART0_FR   ((volatile unsigned int*)(MMIO_BASE + 0x00201018))
#define UART0_IBRD ((volatile unsigned int*)(MMIO_BASE + 0x00201024))
#define UART0_FBRD ((volatile unsigned int*)(MMIO_BASE + 0x00201028))
#define UART0_LCRH ((volatile unsigned int*)(MMIO_BASE + 0x0020102C))
#define UART0_CR   ((volatile unsigned int*)(MMIO_BASE + 0x00201030))
#define UART0_IMSC ((volatile unsigned int*)(MMIO_BASE + 0x00201038))
#define UART0_ICR  ((volatile unsigned int*)(MMIO_BASE + 0x00201044))

//static bool uart_initialized = false;
/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init() {
//  if (uart_initialized) {
//    return;
//  }

  unsigned int r;

  /* initialize UART */
  *UART0_CR = 0; // turn off UART0

  /* set up clock for consistent divisor values */
  mbox[0] = 9 * 4;
  mbox[1] = MBOX_REQUEST;
  mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
  mbox[3] = 12;
  mbox[4] = 8;
  mbox[5] = 2;       // UART clock
  mbox[6] = 4000000; // 4Mhz
  mbox[7] = 0;       // clear turbo
  mbox[8] = MBOX_TAG_LAST;
  mbox_call(MBOX_CH_PROP);

  /* map UART0 to GPIO pins */
  r = *GPFSEL1;
  r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
  r |= (4 << 12) | (4 << 15);    // alt0
  *GPFSEL1 = r;
  *GPPUD   = 0; // enable pins 14 and 15
  r        = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = (1 << 14) | (1 << 15);
  r          = 150;
  while (r--) {
    asm volatile("nop");
  }
  *GPPUDCLK0 = 0; // flush GPIO setup

  *UART0_ICR       = 0x7FF; // clear interrupts
  *UART0_IBRD      = 2;     // 115200 baud
  *UART0_FBRD      = 0xB;
  *UART0_LCRH      = 0b11 << 5; // 8n1
  *UART0_CR        = 0x301;     // enable Tx, Rx, FIFO
//  uart_initialized = true;
}

//bool is_uart_initialized() { return uart_initialized; }

/**
 * Send a character
 */
void uart_send(unsigned int c) {
  /* wait until we can send */
  do {
    asm volatile("nop");
  } while (*UART0_FR & 0x20);
  /* write the character to the buffer */
  *UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc() {
  char r;
  /* wait until something is in the buffer */
  do {
    asm volatile("nop");
  } while (*UART0_FR & 0x10);
  /* read it and return */
  r = (char)(*UART0_DR);
  /* convert carrige return to newline */
  return r == '\r' ? '\n' : r;
}

/**
 * Display a string
 */
void uart_puts(const char* s) {
  while (*s) {
    /* convert newline to carrige return + newline */
    if (*s == '\n')
      uart_send('\r');
    uart_send(*s++);
  }
}

extern_C void test_output() {
  uart_init();
  uart_send(65);
  uart_send('\n');
}

extern_C void print_current_el(unsigned int current_el) {
  switch (current_el) {
  case 1:
    uart_puts("el 1");
    break;
  case 2:
    uart_puts("el 2");
    break;
  case 3:
    uart_puts("el 3");
    break;
  case 0:
    uart_puts("el 0");
    break;
  case 4:
    uart_puts("el 4");
    break;
  }
  uart_puts("current el: ");
  uart_send(current_el + 0x30);
  uart_send('\n');
}

static int debug_count = 0;
extern_C void debug() {
  uart_puts("debug count: ");
  uart_send(debug_count + 0x30);
  uart_send('\n');
  ++debug_count;
}

/**
 * Display a binary value in hexadecimal
 */
void uart_hex(unsigned int d) {
  unsigned int n;
  int c;
  for (c = 28; c >= 0; c -= 4) {
    // get highest tetrad
    n = (d >> c) & 0xF;
    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;
    uart_send(n);
  }
}
