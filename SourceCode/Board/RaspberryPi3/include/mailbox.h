#pragma once

/* a properly aligned buffer */
/* mailbox message buffer */
volatile unsigned int __attribute__((aligned(16))) mailbox[36];

#define MAILBOX_REQUEST 0

/* channels */
#define MAILBOX_CH_POWER 0
#define MAILBOX_CH_FRAMEBUFFER 1
#define MAILBOX_CH_VUART 2
#define MAILBOX_CH_VCHIQ 3
#define MAILBOX_CH_LEDS 4
#define MAILBOX_CH_BUTTONS 5
#define MAILBOX_CH_TOUCH 6
#define MAILBOX_CH_COUNT 7
#define MAILBOX_CH_PROPERTY 8

/* tags */
#define MAILBOX_TAG_GET_SERIAL 0x10004
#define MAILBOX_TAG_SET_POWER 0x28001
#define MAILBOX_TAG_SET_CLOCK_RATE 0x38002
#define MAILBOX_TAG_LAST 0

int mailbox_call(unsigned char channel);

void mailbox_get_serial_number();
