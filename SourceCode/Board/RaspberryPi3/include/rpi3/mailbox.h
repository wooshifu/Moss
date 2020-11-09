#pragma once

#include "libc/types.h"

#include "rpi3/mmio.h"

/**
 *        0       4       8      12      16      20      24      28      32
 *        +-------+-------------------------------------------------------+
 * 0x00   |rd chn |                     read data                         |
 *        +-------+-------------------------------------------------------+
 * 0x04   |                             Unused                            |
 * ...    \\                                                             \\
 * 0x14   |                             Unused                            |
 *        +-----------------------------------------------------------+-+-+
 * 0x18   |      status reserved                                      |E|F|
 *        +-----------------------------------------------------------+-+-+
 * 0x1C   |                             Unused                            |
 *        +-----------------------------------------------------------+-+-+
 * 0x20   |wt chn |                    write data                         |
 *        +-----------------------------------------------------------+-+-+
 */
#define VIDEOCORE_MBOX (MMIO_BASE + 0x0000B880)

#define MAILBOX_CHANNEL_POWER_MANAGEMENT                0
#define MAILBOX_CHANNEL_FRAMEBUFFER                     1
#define MAILBOX_CHANNEL_VIRTUAL_UART                    2
#define MAILBOX_CHANNEL_VCHIQ                           3
#define MAILBOX_CHANNEL_LEDS                            4
#define MAILBOX_CHANNEL_BUTTONS                         5
#define MAILBOX_CHANNEL_TOUCH_SCREEN                    6
#define MAILBOX_CHANNEL_COUNT                           7
#define MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VIDEO_CORE 8
#define MAILBOX_CHANNEL_PROPERTY_TAGS_VIDEO_CORE_TO_ARM 9

/**
 * The read register for mailbox 0 at offset (the Linux source mentions something of "and the next 4 words", but I've
 * found it sufficient to read only from this address) 0-3(channel):	The mailbox channel number from which the data
 * originated 4-31(data):	The 28 bits of data sent to the CPU
 */
#define MAILBOX0_READ ((volatile u32 *)(VIDEOCORE_MBOX + 0x0))

// Read from the mailbox without removing data from it.
#define MAILBOX0_PEAK ((volatile u32 *)(VIDEOCORE_MBOX + 0x10))

// Sender ID (bottom 2 bits only)
#define MAILBOX0_SENDER ((volatile u32 *)(VIDEOCORE_MBOX + 0x14))

/**
 * The status register for mailbox 0
 * 0-29:	N/A	Not documented here. Unused?
 * 30:	MAIL_EMPTY	Set if the mailbox is empty, and thus no more data is available to be read from it.
 * 31:	MAIL_FULL	Set if the mailbox is full, and thus no more data can be written to it.
 */
#define MAILBOX0_STATUS ((volatile u32 *)(VIDEOCORE_MBOX + 0x18))

// The configuration register for mailbox 0
#define MAILBOX0_CONFIG ((volatile u32 *)(VIDEOCORE_MBOX + 0x1C))

/**
 *  The write register for mailbox 0 (this is actually the read register for mailbox 1).
 *  0-3:	channel	The mailbox channel number to which the data is to be sent
 *  4-31:	data	The 28 bits of data to be sent to the destination
 */
#define MAILBOX0_WRITE ((volatile u32 *)(VIDEOCORE_MBOX + 0x20))

#define MAILBOX_CODE_REQUEST          0x00000000
#define MAILBOX_CODE_RESPONSE_SUCCESS 0x80000000
#define MAILBOX_CODE_RESPONSE_FAILURE 0x80000001

//#define MAILBOX_CODE_TAG_REQUEST 0x00000000
#define MAILBOX_CODE_TAG_IS_REQUEST(code) ((code)&0x00000000)
//#define MAILBOX_CODE_TAG_RESPONSE 0x80000000
#define MAILBOX_CODE_TAG_IS_RESPONSE(code) ((code)&0x80000000)

#define MAILBOX_FULL  1 << 31
#define MAILBOX_EMPTY 1 << 30

/**
 * all property interface can be found at below link:
 * https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
 *
 *        0       4       8      12      16      20      24      28      32
 *        +---------------------------------------------------------------+
 * 0x00   |                         Buffer Size                           |
 *        +---------------------------------------------------------------+
 * 0x04   |                   Request/Response Code                       |
 *        +---------------------------------------------------------------+
 * 0x08   |                             Tags                              |
 * ...    \\                                                             \\
 * 0xXX   |                             Tags                              |
 *        +---------------------------------------------------------------+
 * 0xXX+4 |                           End Tag (0)                         |
 *        +---------------------------------------------------------------+
 * 0xXX+8 |                           Padding                             |
 * ...    \\                                                             \\
 * 0xXX+16|                           Padding                             |
 *        +---------------------------------------------------------------+
 *
 * A Tag starts with a 4 byte tag id, which identifies which command this tag wants to run.
 * In general, Tags have form 0x000XYZZZ,
 * where X identifies which hardware device you are accessing,
 * Y identifies which type of command it is (0 = get, 4 = test, 8 = set),
 * and ZZZ identifies the specific command.
 *
 * map of tag:
 *        0       4       8      12      16      20      24      28      32
 *        +---------------------------------------------------------------+
 * 0x00   |                         Tag Identity                          |
 *        +---------------------------------------------------------------+
 * 0x04   |                       Value Buffer Size                       |
 *        +---------------------------------------------------------------+
 * 0x08   |                     Request/Response Code                     |
 *        +---------------------------------------------------------------+
 * 0x0C   |                          Value Buffer                         |
 * ...    \\                                                             \\
 * 0xXX   | Value Buffer  |                    Padding                    |
 *        +---------------------------------------------------------------+
 */
#define MAILBOX_PROPERTY_TAG_END 0x00000000

#define MAILBOX_CLOCK_ID_RESERVED  0x000000000 // reserved
#define MAILBOX_CLOCK_ID_EMMC      0x000000001 // EMMC
#define MAILBOX_CLOCK_ID_UART      0x000000002 // UART
#define MAILBOX_CLOCK_ID_ARM       0x000000003 // ARM
#define MAILBOX_CLOCK_ID_CORE      0x000000004 // CORE
#define MAILBOX_CLOCK_ID_V3D       0x000000005 // V3D
#define MAILBOX_CLOCK_ID_H264      0x000000006 // H264
#define MAILBOX_CLOCK_ID_ISP       0x000000007 // ISP
#define MAILBOX_CLOCK_ID_SDRAM     0x000000008 // SDRAM
#define MAILBOX_CLOCK_ID_PIXEL     0x000000009 // PIXEL
#define MAILBOX_CLOCK_ID_PWM       0x00000000a // PWM
#define MAILBOX_CLOCK_ID_HEVC      0x00000000b // HEVC
#define MAILBOX_CLOCK_ID_EMMC2     0x00000000c // EMMC2
#define MAILBOX_CLOCK_ID_M2MC      0x00000000d // M2MC
#define MAILBOX_CLOCK_ID_PIXEL_BVB 0x00000000e // PIXEL_BVB

#define MAILBOX_PROPERTY_TAG_SET_CLOCK_RATE 0x00038002
// NOTE: don't change the struct member order, it should be this order described by rasberrypi docs
constexpr int mailbox_property_alignment = 16;
using mailbox_property_set_clock_rate_t = struct mailbox_property_set_clock_rate_t {
  u32 size; // =sizeof(struct my_property_set_clock_rate_t);
  u32 code; // =MAILBOX_CODE_REQUEST ;

  u32 tag;                // =MAILBOX_PROPERTY_TAG_SET_CLOCK_RATE;
  u32 buffer_size;        // =12;
  u32 tag_code;           // =MAILBOX_CODE_REQUEST;
  u32 clock_id;           // =CLOCK_ID_UART;
  u32 rate;               // =4000000;
  u32 skip_setting_turbo; // =0;

  u32 end; // =MAILBOX_PROPERTY_TAG_END
};
// The buffer itself is 16-byte aligned as only the upper 28 bits of the address can be passed via the mailbox.

/**
 * send mailbox message to mailbox
 * @param channel which channel to send
 * @param data message data
 */
void mailbox_call(u32 channel, void *data);

/**
 * after mailbox_call, we should detect the response is successful or not
 * @param code code
 * @param tag_code tag code
 * @return true on success, false on failure
 */
bool is_valid_mailbox_response(u32 code, u32 tag_code);
