#include "libcxx/cast.hh"  // for force_cast
#include "rpi3/mailbox.hh" // for call, SetClockRate, Channel, Channel::...
#include "rpi3/mmio.hh"
#include "rpi3/rpi3.hh"  // for NS_RPI3
#include "rpi3/uart0.hh" // for init_uart0

namespace NS_rpi3 {
  /* PL011 UART registers */
  auto* UART0_DATA_REGISTER                     = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201000);
  auto* UART0_FLAG_REGISTER                     = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201018);
  auto* UART0_INTEGER_BAUD_RATE_DIVISOR         = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201024);
  auto* UART0_FRACTIONAL_BAUD_RATE_DIVISOR      = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201028);
  auto* UART0_LINE_CONTROL_REGISTER             = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x0020102C);
  auto* UART0_CONTROL_REGISTER                  = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201030);
  auto* UART0_INTERRUPT_MASK_SET_CLEAR_REGISTER = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201038);
  auto* UART0_INTERRUPT_CLEAR_REGISTER          = force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201044);

  /**
   * Set baud rate and characteristics (115200 8N1) and map to GPIO
   */
  void init_uart0() {
    *UART0_CONTROL_REGISTER = 0; // turn off UART0

    alignas(16) auto set_clock_rate = NS_mailbox::property::SetClockRate(4'000'000, 0);
    bool success = NS_mailbox::call(mailbox::Channel::PROPERTY_TAGS_ARM_TO_VIDEO_CORE, set_clock_rate);
    if (not success) {
      // todo: what should we do if success
    }
    //  log_d("response code: %x, tag code: %x", data.code, data.tag_code);
    // todo: uncomment following line will cause print to stuck
    //  if (!is_valid_mailbox_response(data.code, data.tag_code)) {
    //     NOTE: uart0 not initialized, could we log this message to uart0???
    //    log_f("failed to set clock uart rate, code: %u, tag_code: %u", data.code, data.tag_code);
    //    return;
    //  }
    //  log_d("uart0(id:%u) clock rate initialized to rate:%u", data.clock_id, data.rate);

    /* map UART0 to GPIO pins */
    unsigned int r = *NS_mmio::GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15, [17:15]:FSEL15, [14:12]:FSEL14
    r |= (4 << 12) | (4 << 15);    // alt0, 0b100 = GPIO Pin takes alternate function 0
    *NS_mmio::GPFSEL1 = r;
    *NS_mmio::GPPUD   = 0; // enable pins 14 and 15
    r                 = 150;
    while (r--) {
      asm volatile("nop");
    }
    *NS_mmio::GPPUDCLK0 = (1 << 14) | (1 << 15);
    r                   = 150;
    while (r--) {
      asm volatile("nop");
    }
    *NS_mmio::GPPUDCLK0                 = 0;         // flush GPIO setup
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

  // void uart_send(unsigned int c);

  /**
   * hal putchar implementation
   * @param character the character
   */
  int putchar(int character) {
    uart0_send(character);
    //  uart_send(character);
    return 0;
  }

  int getchar() { return uart0_getc(); }

} // namespace NS_rpi3

int putchar(int character) { return NS_rpi3::putchar(character); }

#if 0
/* PL011 UART registers */
#define UART0_DR   force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201000)
#define UART0_FR   force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201018)
#define UART0_IBRD force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201024)
#define UART0_FBRD force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201028)
#define UART0_LCRH force_cast(volatile unsigned int*, NS_mmio::BASE + 0x0020102C)
#define UART0_CR   force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201030)
#define UART0_IMSC force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201038)
#define UART0_ICR  force_cast(volatile unsigned int*, NS_mmio::BASE + 0x00201044)

// static bool uart_initialized = false;
/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init() {
  /* initialize UART */
  *UART0_CR = 0; // turn off UART0

  /* set up clock for consistent divisor values */
  mbox[0] = 9 * 4;
  mbox[1] = MBOX_REQUEST;
  mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
  mbox[3] = 12;
  mbox[4] = 8;
  mbox[5] = 2;       // UART clock
  mbox[6] = 4'000'000; // 4Mhz
  mbox[7] = 0;       // clear turbo
  mbox[8] = MBOX_TAG_LAST;
  mbox_call(MBOX_CH_PROP);

  /* map UART0 to GPIO pins */
  unsigned int r = *GPFSEL1;
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

  *UART0_ICR  = 0x7FF;     // clear interrupts
  *UART0_IBRD = 2;         // 115200 baud
  *UART0_FBRD = 0xB;       // fbrd
  *UART0_LCRH = 0b11 << 5; // 8n1
  *UART0_CR   = 0x301;     // enable Tx, Rx, FIFO
}

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
  char r = 0;
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
    /* convert newline to carriage return + newline */
    if (*s == '\n') {
      uart_send('\r');
    }
    uart_send(*s++);
  }
}

    /**
 * Display a binary value in hexadecimal
 */
void uart_hex(unsigned int d) {
  unsigned int n = 0;
  int c          = 0;
  for (c = 28; c >= 0; c -= 4) {
    // get highest tetrad
    n = (d >> c) & 0xF;
    // 0-9 => '0'-'9', 10-15 => 'A'-'F'
    n += n > 9 ? 0x37 : 0x30;
    uart_send(n);
  }
}
#endif
