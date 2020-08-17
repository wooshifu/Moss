#if 0
#pragma once

/* a properly aligned buffer */
/* mailbox message buffer */
volatile unsigned int __attribute__((aligned(16))) mailbox[36];

#define MAILBOX_REQUEST 0
#define MAILBOX_CODE_REQUEST 0x00000000
#define MAILBOX_CODE_RESPONSE_SUCCESS 0x80000000
#define MAILBOX_CODE_RESPONSE_FAILURE 0x80000001

/* channels */
#define MAILBOX_CHANNEL_POWER 0
#define MAILBOX_CHANNEL_FRAMEBUFFER 1
#define MAILBOX_CHANNEL_VUART 2
#define MAILBOX_CHANNEL_VCHIQ 3
#define MAILBOX_CHANNEL_LEDS 4
#define MAILBOX_CHANNEL_BUTTONS 5
#define MAILBOX_CHANNEL_TOUCH 6
#define MAILBOX_CHANNEL_COUNT 7
#define MAILBOX_CHANNEL_PROPERTY_ARM_TO_VIDEO_CORE 8
#define MAILBOX_CHANNEL_PROPERTY_VIDEO_CORE_TO_ARM 9

/* tags */
#define MAILBOX_TAG_GET_SERIAL 0x10004
#define MAILBOX_TAG_SET_POWER 0x28001
#define MAILBOX_TAG_SET_CLOCK_RATE 0x38002
#define MAILBOX_TAG_LAST 0

int mailbox_call(unsigned char channel);

void mailbox_get_serial_number();

#endif