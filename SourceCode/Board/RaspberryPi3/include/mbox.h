#pragma once

#include "gpio.h"

#include <stdint.h>

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

#define MAILBOX_CHANNEL_POWER_MANAGEMENT 0
#define MAILBOX_CHANNEL_FRAMEBUFFER 1
#define MAILBOX_CHANNEL_VIRTUAL_UART 2
#define MAILBOX_CHANNEL_VCHIQ 3
#define MAILBOX_CHANNEL_LEDS 4
#define MAILBOX_CHANNEL_BUTTONS 5
#define MAILBOX_CHANNEL_TOUCH_SCREEN 6
#define MAILBOX_CHANNEL_COUNT 7
#define MAILBOX_CHANNEL_PROPERTY_TAGS_ARM_TO_VIDEO_CORE 8
#define MAILBOX_CHANNEL_PROPERTY_TAGS_VIDEO_CORE_TO_ARM 9

/**
 * The read register for mailbox 0 at offset (the Linux source mentions something of "and the next 4 words", but I've
 * found it sufficient to read only from this address) 0-3(channel):	The mailbox channel number from which the data
 * originated 4-31(data):	The 28 bits of data sent to the CPU
 */
#define MAILBOX0_READ ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x0))

// Read from the mailbox without removing data from it.
#define MAILBOX0_PEAK ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x10))

// Sender ID (bottom 2 bits only)
#define MAILBOX0_SENDER ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x14))

/**
 * The status register for mailbox 0
 * 0-29:	N/A	Not documented here. Unused?
 * 30:	MAIL_EMPTY	Set if the mailbox is empty, and thus no more data is available to be read from it.
 * 31:	MAIL_FULL	Set if the mailbox is full, and thus no more data can be written to it.
 */
#define MAILBOX0_STATUS ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x18))

// The configuration register for mailbox 0
#define MAILBOX0_CONFIG ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x1C))

/**
 *  The write register for mailbox 0 (this is actually the read register for mailbox 1).
 *  0-3:	channel	The mailbox channel number to which the data is to be sent
 *  4-31:	data	The 28 bits of data to be sent to the destination
 */
#define MAILBOX0_WRITE ((volatile uint32_t *)(VIDEOCORE_MBOX + 0x20))

#define MAILBOX_CODE_REQUEST 0x00000000
#define MAILBOX_CODE_RESPONSE_SUCCESS 0x80000000
#define MAILBOX_CODE_RESPONSE_FAILURE 0x80000001

//#define MAILBOX_CODE_TAG_REQUEST 0x00000000
#define MAILBOX_CODE_TAG_IS_REQUEST(code) ((code) & 0x00000000)
//#define MAILBOX_CODE_TAG_RESPONSE 0x80000000
#define MAILBOX_CODE_TAG_IS_RESPONSE(code) ((code) & 0x80000000)

#define MAILBOX_FULL 1 << 31
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
#define PROPERTY_TAG_END 0x00000000

#define PROPERTY_TAG_GET_FIRMWARE_REVISION 0x00000001
typedef struct property_get_firmware_revision_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t firmware_revision;
} property_get_firmware_revision_t;

#define PROPERTY_TAG_GET_BOARD_MODEL 0x00010001
typedef struct property_get_board_model_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t board_model;
} property_get_board_model_t;

#define PROPERTY_TAG_GET_BOARD_REVISION 0x00010002
typedef struct property_get_board_revision_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t board_revision;
} property_get_board_revision_t;

#define PROPERTY_TAG_GET_BOARD_MAC_ADDRESS 0x00010003
typedef struct property_get_board_mac_address_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint8_t address_1;
  uint8_t address_2;
  uint8_t address_3;
  uint8_t address_4;
  uint8_t address_5;
  uint8_t address_6;
} property_get_board_mac_address_t;

#define PROPERTY_TAG_GET_BOARD_SERIAL 0x00010004
typedef struct property_get_board_serial_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint64_t board_serial;
} property_get_board_serial_t;

#define PROPERTY_TAG_GET_ARM_MEMORY 0x00010005
typedef struct property_get_arm_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t base_address;
  uint32_t size;
} property_get_arm_memory_t;

#define PROPERTY_TAG_GET_VIDEOCORE_MEMORY 0x00010006
typedef struct property_get_video_core_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t base_address;
  uint32_t size;
} property_get_video_core_memory_t;

#define PROPERTY_TAG_GET_CLOCKS 0x00010007
typedef struct property_get_clocks_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t parent_clock_id;
  uint32_t clock_id;
  // ...
} property_get_clocks_t;

#define PROPERTY_TAG_GET_COMMAND_LINE 0x00050001
typedef struct property_get_command_line_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint8_t command_line_str;
  // ...
} property_get_command_line_t;

#define PROPERTY_TAG_GET_DMA_CHANNELS 0x00060001
typedef struct property_get_dmachannels_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t mask; // Bits 0-15: DMA channels 0-15 (0=do not use, 1=usable),Bits 16-31: reserved for future use
} property_get_dmachannels_t;

#define DEVICE_ID_SD_Card 0x00000000
#define DEVICE_ID_UART0 0x00000001
#define DEVICE_ID_UART1 0x00000002
#define DEVICE_ID_USB_HCD 0x00000003
#define DEVICE_ID_I2C0 0x00000004
#define DEVICE_ID_I2C1 0x00000005
#define DEVICE_ID_I2C2 0x00000006
#define DEVICE_ID_SPI 0x00000007
#define DEVICE_ID_CCP2TX 0x00000008
#define DEVICE_ID_Unknown1_RPi4 0x00000009
#define DEVICE_ID_Unknown2_RPi4 0x0000000a

#define PROPERTY_TAG_GET_POWER_STATE 0x00020001
typedef struct property_get_power_state_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t device_id;
  /**
   * Bit 0: 0=off, 1=on
   * Bit 1: 0=device exists, 1=device does not exist
   * Bits 2-31: reserved for future use
   */
  uint32_t state;
} property_get_power_state_t;

#define PROPERTY_TAG_GET_TIMING 0x00020002
typedef struct property_get_timing_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t device_id;
  uint32_t wait_time;
} property_get_timing_t;

#define PROPERTY_TAG_SET_POWER_STATE 0x00028001
typedef struct property_set_power_state_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t device_id;
  /**
   * Bit 0: 0=off, 1=on
   * Bit 1: 0=device exists, 1=device does not exist
   * Bits 2-31: reserved for future use
   */
  uint32_t response_state;
} property_set_power_state_t;

#define CLOCK_ID_reserved 0x000000000
#define CLOCK_ID_EMMC 0x000000001
#define CLOCK_ID_UART 0x000000002
#define CLOCK_ID_ARM 0x000000003
#define CLOCK_ID_CORE 0x000000004
#define CLOCK_ID_V3D 0x000000005
#define CLOCK_ID_H264 0x000000006
#define CLOCK_ID_ISP 0x000000007
#define CLOCK_ID_SDRAM 0x000000008
#define CLOCK_ID_PIXEL 0x000000009
#define CLOCK_ID_PWM 0x00000000a
#define CLOCK_ID_HEVC 0x00000000b
#define CLOCK_ID_EMMC2 0x00000000c
#define CLOCK_ID_M2MC 0x00000000d
#define CLOCK_ID_PIXEL_BVB 0x00000000e

#define PROPERTY_TAG_GET_CLOCK_STATE 0x00030001
typedef struct property_get_clock_state_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  /**
   * Bit 0: 0=off, 1=on
   * Bit 1: 0=clock exists, 1=clock does not exist
   * Bits 2-31: reserved for future use
   */
  uint32_t response_state;
} property_get_clock_state_t;

#define PROPERTY_TAG_SET_CLOCK_STATE 0x00038001
typedef struct property_set_clock_state_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  /**
   * Bit 0: 0=off, 1=on
   * Bit 1: 0=clock exists, 1=clock does not exist
   * Bits 2-31: reserved for future use (set to 0)
   */
  uint32_t state;
} property_set_clock_state_t;

#define PROPERTY_TAG_GET_CLOCK_RATE 0x00030002
typedef struct property_get_clock_rate_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  uint32_t rate;
} property_get_clock_rate_t;

#define PROPERTY_TAG_SET_CLOCK_RATE 0x00038002
typedef struct property_set_clock_rate_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  uint32_t rate;
  uint32_t skip_setting_turbo;
} property_set_clock_rate_t;

#define PROPERTY_TAG_GET_MAX_CLOCK_RATE 0x00030004
typedef struct property_get_max_clock_rate_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  uint32_t rate;
} property_get_max_clock_rate_t;

#define PROPERTY_TAG_GET_MIN_CLOCK_RATE 0x00030007
typedef struct property_get_min_clock_rate_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t clock_id;
  uint32_t rate;
} property_get_min_clock_rate_t;

#define PROPERTY_TAG_GET_TURBO 0x00030009
typedef struct property_get_turbo_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t id;
  uint32_t level;
} property_get_turbo_t;

#define PROPERTY_TAG_SET_TURBO 0x00038009
typedef struct property_set_turbo_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t id;
  uint32_t level;
} property_set_turbo_t;

#define VOLTAGE_IDreserved 0x000000000
#define VOLTAGE_IDCore 0x000000001
#define VOLTAGE_IDSDRAM_C 0x000000002
#define VOLTAGE_IDSDRAM_P 0x000000003
#define VOLTAGE_IDSDRAM_I 0x000000004

#define PROPERTY_TAG_GET_VOLTAGE 0x00030003
typedef struct property_get_voltage_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t voltage_id;
  uint32_t value;
} property_get_voltage_t;

#define PROPERTY_TAG_SET_VOLTAGE 0x00038003
typedef struct property_set_voltage_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t voltage_id;
  uint32_t value;
} property_set_voltage_t;

#define PROPERTY_TAG_GET_MAX_VOLTAGE 0x00030005
typedef struct property_get_max_voltage_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t voltage_id;
  uint32_t value;
} property_get_max_voltage_t;

#define PROPERTY_TAG_GET_MIN_VOLTAGE 0x00030008
typedef struct property_get_min_voltage_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t voltage_id;
  uint32_t value;
} property_get_min_voltage_t;

#define PROPERTY_TAG_GET_TEMPERATURE 0x00030006
typedef struct property_get_temperature_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t temperature_id;
  uint32_t value;
} property_get_temperature_t;

#define PROPERTY_TAG_GET_MAX_TEMPERATURE 0x0003000a
typedef struct property_get_max_temperature_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t temperature_id;
  uint32_t value;
} property_get_max_temperature_t;

#define MEM_FLAG_DISCARDABLE 1 << 0 //  can be resized to 0 at any time. Use for cached data
#define MEM_FLAG_NORMAL 0 << 2 //  normal allocating alias. Don't use from ARM
#define MEM_FLAG_DIRECT 1 << 2 //  0xC alias uncached
#define MEM_FLAG_COHERENT 2 << 2 //  0x8 alias. Non-allocating in L2 but coherent
#define MEM_FLAG_L1_NONALLOCATING (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT) //  Allocating in L2
#define MEM_FLAG_ZERO 1 << 4 //   initialise buffer to all zeros
#define MEM_FLAG_NO_INIT 1 << 5 //  don't initialise (default is initialise to all ones
#define MEM_FLAG_HINT_PERMALOCK 1 << 6 //  Likely to be locked for long periods of time.

#define PROPERTY_TAG_ALLOCATE_MEMORY 0x0003000c
typedef struct property_allocate_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t size;
  uint32_t alignment;
  uint32_t flags;
  uint32_t handle;
} property_allocate_memory_t;

#define PROPERTY_TAG_LOCK_MEMORY 0x0003000d
typedef struct property_lock_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t handle;
  uint32_t bus_address;
} property_lock_memory_t;

#define PROPERTY_TAG_UNLOCK_MEMORY 0x0003000e
typedef struct property_un_lock_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t handle;
  uint32_t status;
} property_un_lock_memory_t;

#define PROPERTY_TAG_RELEASE_MEMORY 0x0003000f
typedef struct property_release_memory_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t handle;
  uint32_t status;
} property_release_memory_t;

#define PROPERTY_TAG_EXECUTE_CODE 0x00030010
typedef struct property_execute_code_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t function_pointer;
  uint32_t r_0;
  uint32_t r_1;
  uint32_t r_2;
  uint32_t r_3;
  uint32_t r_4;
  uint32_t r_5;
} property_execute_code_t;

#define PROPERTY_TAG_GET_DISPMANX_RESOURCE_MEN_HANDLER 0x00030014
typedef struct property_get_dispmanx_resource_mem_handle_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t dispmanx_resource_handle;
  uint32_t value;
  uint32_t mem_handle_for_resource;
} property_get_dispmanx_resource_mem_handle_t;

#define PROPERTY_TAG_GET_EDID_CLOCK 0x00030020
typedef struct property_get_edidblock_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t block_number;
  uint32_t status;
  uint8_t edidblock[128];
} property_get_edidblock_t;

#define PROPERTY_TAG_ALLOCATE_BUFFER 0x00040001
typedef struct property_allocate_buffer_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  union {
    uint32_t alignment;
    uint32_t base_address;
  } PACKED;
  uint32_t size;
} property_allocate_buffer_t;

#define PROPERTY_TAG_RELEASE_BUFFER 0x00048001
typedef struct property_release_buffer_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
} property_release_buffer_t;

#define PROPERTY_TAG_BLANK_SCREEN 0x00040002
typedef struct property_blank_screen_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * Bit 0: 0=off, 1=on
   * Bits 1-31: reserved for future use (set to 0)
   */
  uint32_t state;
} property_blank_screen_t;

#define PROPERTY_TAG_GET_PHYSICAL_DISPLAY_WH 0x00040003
typedef struct property_get_physical_display_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_get_physical_display_wh_t;

#define PROPERTY_TAG_TEST_PHYSICAL_DISPLAY_WH 0x00044003
typedef struct property_test_physical_display_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_test_physical_display_wh_t;

#define PROPERTY_TAG_SET_PHYSICAL_DISPLAY_WH 0x00048003
typedef struct property_set_physical_display_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_set_physical_display_wh_t;

#define PROPERTY_TAG_GET_VIRTUAL_BUFFER_WH 0x00040004
typedef struct property_get_virtual_buffer_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_get_virtual_buffer_wh_t;

#define PROPERTY_TAG_TEST_VIRTUAL_BUFFER_WH 0x00044004
typedef struct property_test_virtual_buffer_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_test_virtual_buffer_wh_t;

#define PROPERTY_TAG_SET_VIRTUAL_BUFFER_WH 0x00048004
typedef struct property_set_virtual_buffer_wh_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
} property_set_virtual_buffer_wh_t;

#define PROPERTY_TAG_GET_DEPTH 0x00040005
typedef struct property_get_depth_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t bits_per_pixel;
} property_get_depth_t;

#define PROPERTY_TAG_TEST_DEPTH 0x00044005
typedef struct property_test_depth_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t bits_per_pixel;
} property_test_depth_t;

#define PROPERTY_TAG_SET_DEPTH 0x00048005
typedef struct property_set_depth_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t bits_per_pixel;
} property_set_depth_t;

#define PROPERTY_TAG_GET_PIXEL_ORDER 0x00040006
typedef struct property_get_pixel_order_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * 0x0: BGR
   * 0x1: RGB
   */
  uint32_t state;
} property_get_pixel_order_t;

#define PROPERTY_TAG_TEST_PIXEL_ORDER 0x00044006
typedef struct property_test_pixel_order_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * 0x0: BGR
   * 0x1: RGB
   */
  uint32_t state;
} property_test_pixel_order_t;

#define PROPERTY_TAG_SET_PIXEL_ORDER 0x00048006
typedef struct property_set_pixel_order_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * 0x0: BGR
   * 0x1: RGB
   */
  uint32_t state;
} property_set_pixel_order_t;

#define PROPERTY_TAG_GET_ALPHA_MODE 0x00040007
typedef struct property_get_alpha_mode_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * 0x0: Alpha channel enabled (0 = fully opaque)
   * 0x1: Alpha channel reversed (0 = fully transparent)
   * 0x2: Alpha channel ignored
   */
  uint32_t state;
} property_get_alpha_mode_t;

#define PROPERTY_TAG_TEST_ALPHA_MODE 0x00044007
typedef struct property_test_alpha_mode_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t state;
} property_test_alpha_mode_t;

#define PROPERTY_TAG_SET_ALPHA_MODE 0x00048007
typedef struct property_set_alpha_mode_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t state;
} property_set_alpha_mode_t;

#define PROPERTY_TAG_GET_PITCH 0x00040008
typedef struct property_get_pitch_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t bytes_per_line;
} property_get_pitch_t;

#define PROPERTY_TAG_GET_VIRTUAL_OFFSET 0x00040009
typedef struct property_get_virtual_offset_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t x_offset;
  uint32_t y_offset;
} property_get_virtual_offset_t;

#define PROPERTY_TAG_TEST_VIRTUAL_OFFSET 0x00044009
typedef struct property_test_virtual_offset_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t x_offset;
  uint32_t y_offset;
} property_test_virtual_offset_t;

#define PROPERTY_TAG_SET_VIRTUAL_OFFSET 0x00048009
typedef struct property_set_virtual_offset_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t x_offset;
  uint32_t y_offset;
} property_set_virtual_offset_t;

#define PROPERTY_TAG_GET_OVERSCAN 0x0004000a
typedef struct property_get_overscan_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t top;
  uint32_t bottom;
  uint32_t left;
  uint32_t right;
} property_get_overscan_t;

#define PROPERTY_TAG_TEST_OVERSCAN 0x0004400a
typedef struct property_test_overscan_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t top;
  uint32_t bottom;
  uint32_t left;
  uint32_t right;
} property_test_overscan_t;

#define PROPERTY_TAG_SET_OVERSCAN 0x0004800a
typedef struct property_set_overscan_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t top;
  uint32_t bottom;
  uint32_t left;
  uint32_t right;
} property_set_overscan_t;

#define PROPERTY_TAG_GET_PALETTE 0x0004000b
typedef struct property_get_palette_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t palette[256];
} property_get_palette_t;

#define PROPERTY_TAG_TEST_PALETTE 0x0004400b
typedef struct property_test_palette_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t first;
  uint32_t length;
  uint32_t palette[256]; // (offset to offset+length-1)
  /**
   * 0=valid,
   * 1=invalid
   */
  uint32_t value;
} property_test_palette_t;

#define PROPERTY_TAG_SET_PALETTE 0x0004800b
typedef struct property_set_palette_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t first;
  uint32_t length;
  uint32_t palette[256]; // (offset to offset+length-1)
  /**
   * 0=valid,
   * 1=invalid
   */
  uint32_t value;
} property_set_palette_t;

#define PROPERTY_TAG_SET_CURSOR_INFO 0x00008010
typedef struct property_set_cursor_info_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  uint32_t width;
  uint32_t height;
  uint32_t unused;
  uint32_t pointer_to_pixels;
  uint32_t hot_spot_x;
  uint32_t hot_spot_y;
  /**
   * 0=valid,
   * 1=invalid
   */
  uint32_t value;
} property_set_cursor_info_t;

#define PROPERTY_TAG_SET_CURSOR_STATE 0x00008011
typedef struct property_set_cursor_state_t {
  uint32_t tag;
  uint32_t buffer_size;
  uint32_t code;
  /**
   * 0=invisable,
   * 1=visable
   */
  uint32_t enable;
  uint32_t x;
  uint32_t y;
  uint32_t flags;
  /**
   * 0=valid,
   * 1=invalid
   */
  uint32_t value;
} property_set_cursor_state_t;

typedef struct property_get_firmware_revision_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_firmware_revision_t property;
  uint32_t end;
} property_get_firmware_revision_mail_t;
typedef struct property_get_board_model_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_board_model_t property;
  uint32_t end;
} property_get_board_model_mail_t;
typedef struct property_get_board_revision_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_board_revision_t property;
  uint32_t end;
} property_get_board_revision_mail_t;
typedef struct property_get_board_mac_address_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_board_mac_address_t property;
  uint32_t end;
} property_get_board_mac_address_mail_t;
typedef struct property_get_board_serial_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_board_serial_t property;
  uint32_t end;
} property_get_board_serial_mail_t;
typedef struct property_get_arm_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_arm_memory_t property;
  uint32_t end;
} property_get_arm_memory_mail_t;
typedef struct property_get_video_core_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_video_core_memory_t property;
  uint32_t end;
} property_get_video_core_memory_mail_t;
typedef struct property_get_clocks_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_clocks_t property;
  uint32_t end;
} property_get_clocks_mail_t;
typedef struct property_get_command_line_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_command_line_t property;
  uint32_t end;
} property_get_command_line_mail_t;
typedef struct property_get_dmachannels_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_dmachannels_t property;
  uint32_t end;
} property_get_dmachannels_mail_t;
typedef struct property_get_power_state_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_power_state_t property;
  uint32_t end;
} property_get_power_state_mail_t;
typedef struct property_get_timing_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_timing_t property;
  uint32_t end;
} property_get_timing_mail_t;
typedef struct property_set_power_state_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_power_state_t property;
  uint32_t end;
} property_set_power_state_mail_t;
typedef struct property_get_clock_state_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_clock_state_t property;
  uint32_t end;
} property_get_clock_state_mail_t;
typedef struct property_set_clock_state_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_clock_state_t property;
  uint32_t end;
} property_set_clock_state_mail_t;
typedef struct property_get_clock_rate_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_clock_rate_t property;
  uint32_t end;
} property_get_clock_rate_mail_t;
typedef struct property_set_clock_rate_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_clock_rate_t property;
  uint32_t end;
} __attribute__((aligned(16))) property_set_clock_rate_mail_t; // The buffer itself is 16-byte aligned as only the upper
                                                               // 28 bits of the address can be passed via the mailbox.
typedef struct property_get_max_clock_rate_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_max_clock_rate_t property;
  uint32_t end;
} property_get_max_clock_rate_mail_t;
typedef struct property_get_min_clock_rate_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_min_clock_rate_t property;
  uint32_t end;
} property_get_min_clock_rate_mail_t;
typedef struct property_get_turbo_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_turbo_t property;
  uint32_t end;
} property_get_turbo_mail_t;
typedef struct property_set_turbo_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_turbo_t property;
  uint32_t end;
} property_set_turbo_mail_t;
typedef struct property_get_voltage_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_voltage_t property;
  uint32_t end;
} property_get_voltage_mail_t;
typedef struct property_set_voltage_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_voltage_t property;
  uint32_t end;
} property_set_voltage_mail_t;
typedef struct property_get_max_voltage_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_max_voltage_t property;
  uint32_t end;
} property_get_max_voltage_mail_t;
typedef struct property_get_min_voltage_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_min_voltage_t property;
  uint32_t end;
} property_get_min_voltage_mail_t;
typedef struct property_get_temperature_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_temperature_t property;
  uint32_t end;
} property_get_temperature_mail_t;
typedef struct property_get_max_temperature_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_max_temperature_t property;
  uint32_t end;
} property_get_max_temperature_mail_t;
typedef struct property_allocate_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_allocate_memory_t property;
  uint32_t end;
} property_allocate_memory_mail_t;
typedef struct property_lock_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_lock_memory_t property;
  uint32_t end;
} property_lock_memory_mail_t;
typedef struct property_un_lock_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_un_lock_memory_t property;
  uint32_t end;
} property_un_lock_memory_mail_t;
typedef struct property_release_memory_mail_t {
  uint32_t size;
  uint32_t code;
  property_release_memory_t property;
  uint32_t end;
} property_release_memory_mail_t;
typedef struct property_execute_code_mail_t {
  uint32_t size;
  uint32_t code;
  property_execute_code_t property;
  uint32_t end;
} property_execute_code_mail_t;
typedef struct property_get_dispmanx_resource_mem_handle_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_dispmanx_resource_mem_handle_t property;
  uint32_t end;
} property_get_dispmanx_resource_mem_handle_mail_t;
typedef struct property_get_edidblock_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_edidblock_t property;
  uint32_t end;
} property_get_edidblock_mail_t;
typedef struct property_allocate_buffer_mail_t {
  uint32_t size;
  uint32_t code;
  property_allocate_buffer_t property;
  uint32_t end;
} property_allocate_buffer_mail_t;
typedef struct property_release_buffer_mail_t {
  uint32_t size;
  uint32_t code;
  property_release_buffer_t property;
  uint32_t end;
} property_release_buffer_mail_t;
typedef struct property_blank_screen_mail_t {
  uint32_t size;
  uint32_t code;
  property_blank_screen_t property;
  uint32_t end;
} property_blank_screen_mail_t;
typedef struct property_get_physical_display_whmail_t {
  uint32_t size;
  uint32_t code;
  property_get_physical_display_wh_t property;
  uint32_t end;
} property_get_physical_display_whmail_t;
typedef struct property_test_physical_display_whmail_t {
  uint32_t size;
  uint32_t code;
  property_test_physical_display_wh_t property;
  uint32_t end;
} property_test_physical_display_whmail_t;
typedef struct property_set_physical_display_whmail_t {
  uint32_t size;
  uint32_t code;
  property_set_physical_display_wh_t property;
  uint32_t end;
} property_set_physical_display_whmail_t;
typedef struct property_get_virtual_buffer_whmail_t {
  uint32_t size;
  uint32_t code;
  property_get_virtual_buffer_wh_t property;
  uint32_t end;
} property_get_virtual_buffer_whmail_t;
typedef struct property_test_virtual_buffer_whmail_t {
  uint32_t size;
  uint32_t code;
  property_test_virtual_buffer_wh_t property;
  uint32_t end;
} property_test_virtual_buffer_whmail_t;
typedef struct property_set_virtual_buffer_whmail_t {
  uint32_t size;
  uint32_t code;
  property_set_virtual_buffer_wh_t property;
  uint32_t end;
} property_set_virtual_buffer_whmail_t;
typedef struct property_get_depth_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_depth_t property;
  uint32_t end;
} property_get_depth_mail_t;
typedef struct property_test_depth_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_depth_t property;
  uint32_t end;
} property_test_depth_mail_t;
typedef struct property_set_depth_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_depth_t property;
  uint32_t end;
} property_set_depth_mail_t;
typedef struct property_get_pixel_order_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_pixel_order_t property;
  uint32_t end;
} property_get_pixel_order_mail_t;
typedef struct property_test_pixel_order_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_pixel_order_t property;
  uint32_t end;
} property_test_pixel_order_mail_t;
typedef struct property_set_pixel_order_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_pixel_order_t property;
  uint32_t end;
} property_set_pixel_order_mail_t;
typedef struct property_get_alpha_mode_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_alpha_mode_t property;
  uint32_t end;
} property_get_alpha_mode_mail_t;
typedef struct property_test_alpha_mode_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_alpha_mode_t property;
  uint32_t end;
} property_test_alpha_mode_mail_t;
typedef struct property_set_alpha_mode_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_alpha_mode_t property;
  uint32_t end;
} property_set_alpha_mode_mail_t;
typedef struct property_get_pitch_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_pitch_t property;
  uint32_t end;
} property_get_pitch_mail_t;
typedef struct property_get_virtual_offset_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_virtual_offset_t property;
  uint32_t end;
} property_get_virtual_offset_mail_t;
typedef struct property_test_virtual_offset_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_virtual_offset_t property;
  uint32_t end;
} property_test_virtual_offset_mail_t;
typedef struct property_set_virtual_offset_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_virtual_offset_t property;
  uint32_t end;
} property_set_virtual_offset_mail_t;
typedef struct property_get_overscan_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_overscan_t property;
  uint32_t end;
} property_get_overscan_mail_t;
typedef struct property_test_overscan_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_overscan_t property;
  uint32_t end;
} property_test_overscan_mail_t;
typedef struct property_set_overscan_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_overscan_t property;
  uint32_t end;
} property_set_overscan_mail_t;
typedef struct property_get_palette_mail_t {
  uint32_t size;
  uint32_t code;
  property_get_palette_t property;
  uint32_t end;
} property_get_palette_mail_t;
typedef struct property_test_palette_mail_t {
  uint32_t size;
  uint32_t code;
  property_test_palette_t property;
  uint32_t end;
} property_test_palette_mail_t;
typedef struct property_set_palette_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_palette_t property;
  uint32_t end;
} property_set_palette_mail_t;
typedef struct property_set_cursor_info_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_cursor_info_t property;
  uint32_t end;
} property_set_cursor_info_mail_t;
typedef struct property_set_cursor_state_mail_t {
  uint32_t size;
  uint32_t code;
  property_set_cursor_state_t property;
  uint32_t end;
} property_set_cursor_state_mail_t;

// The mailbox interface has 28 bits (MSB) available for the value and 4 bits (LSB) for the channel
typedef struct mailbox_message_t {
  uint8_t channel : 4;
  uint32_t data : 28; // pointer to data
} mailbox_message_t;

#ifndef CAST_TO_MAILBOX_DATA
#define CAST_TO_MAILBOX_DATA(property_mail_t) *((uint32_t *)&(property_mail_t))
#endif

//typedef struct mailbox_status_t {
//  uint32_t reserved : 30;
//  uint8_t empty : 1;
//  uint8_t full : 1;
//} mailbox_status_t;

//mailbox_message_t mailbox_call(mailbox_message_t msg);
//mailbox_message_t mailbox_read(uint8_t channel);
void mailbox_send(mailbox_message_t* message);
