#include "gpio.h"
#include "log.h"
#include "mailbox.h"

#define VIDEOCORE_MAILBOX (MMIO_BASE + 0x0000B880)
#define MAILBOX_READ ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x0))
#define MAILBOX_POLL ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x10))
#define MAILBOX_SENDER ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x14))
#define MAILBOX_STATUS ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x18))
#define MAILBOX_CONFIG ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x1C))
#define MAILBOX_WRITE ((volatile unsigned int *)(VIDEOCORE_MAILBOX + 0x20))
#define MAILBOX_RESPONSE 0x80000000
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mailbox_call(unsigned char channel) {
  unsigned int r = (((unsigned int)((unsigned long)&mailbox) & ~0xF) | (channel & 0xF));
  /* wait until we can write to the mailbox */
  do {
    asm volatile("nop");
  } while ((*MAILBOX_STATUS) & MAILBOX_FULL);
  /* write the address of our message to the mailbox with channel identifier */
  *MAILBOX_WRITE = r;
  /* now wait for the response */
  while (1) {
    /* is there a response? */
    do {
      asm volatile("nop");
    } while (*MAILBOX_STATUS & MAILBOX_EMPTY);
    /* is it a response to our message? */
    if (r == *MAILBOX_READ) {
      /* is it a valid successful response? */
      return mailbox[1] == MAILBOX_RESPONSE;
    }
  }
  return 0;
}

void mailbox_get_serial_number() { // get the board's unique serial number with a mailbox call
  mailbox[0] = 8 * 4;              // length of the message
  mailbox[1] = MAILBOX_REQUEST;    // this is a request message

  mailbox[2] = MAILBOX_TAG_GET_SERIAL; // get serial number command
  mailbox[3] = 8;                      // buffer size
  mailbox[4] = 8;
  mailbox[5] = 0; // clear output buffer
  mailbox[6] = 0;

  mailbox[7] = MAILBOX_TAG_LAST;

  // send the message to the GPU and receive answer
  if (mailbox_call(MAILBOX_CH_PROPERTY) != 0) {
    log_i("serial number: %d%d", mailbox[6], mailbox[5]);
  } else {
    log_e("Unable to query serial!");
  }
}
