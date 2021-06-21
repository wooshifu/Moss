#pragma once

#include "concepts"
#include "libcxx/types.hh"
#include "libcxx/utils.hh"
#include "rpi3/mmio.hh"
#include "rpi3/rpi3.hh"
#include "type_traits"

#ifndef NS_mailbox
#define NS_mailbox mailbox
#endif

namespace NS_rpi3::NS_mailbox {
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

  constexpr auto VIDEOCORE_MBOX = NS_mmio::BASE + 0x0000B880;

  /**
   * The read register for mailbox 0 at offset (the Linux source mentions something of "and the next 4 words", but I've
   * found it sufficient to read only from this address) 0-3(channel):	The mailbox channel number from which the data
   * originated 4-31(data):	The 28 bits of data sent to the CPU
   */
  static auto* MAILBOX0_READ = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x0);

  // Read from the mailbox without removing data from it.
  static auto* MAILBOX0_PEAK = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x10);

  // Sender ID (bottom 2 bits only)
  static auto* MAILBOX0_SENDER = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x14);

  /**
   * The status register for mailbox 0
   * 0-29:	N/A	Not documented here. Unused?
   * 30:	MAIL_EMPTY	Set if the mailbox is empty, and thus no more data is available to be read from it.
   * 31:	MAIL_FULL	Set if the mailbox is full, and thus no more data can be written to it.
   */
  static auto* MAILBOX0_STATUS = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x18);

  // The configuration register for mailbox 0
  static auto* MAILBOX0_CONFIG = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x1C);

  /**
   *  The write register for mailbox 0 (this is actually the read register for mailbox 1).
   *  0-3:	channel	The mailbox channel number to which the data is to be sent
   *  4-31:	data	The 28 bits of data to be sent to the destination
   */
  static auto* MAILBOX0_WRITE = force_cast(volatile u32*, VIDEOCORE_MBOX + 0x20);

  constexpr auto CODE_REQUEST          = 0x00000000;
  constexpr auto CODE_RESPONSE_SUCCESS = 0x80000000;
  constexpr auto CODE_RESPONSE_FAILURE = 0x80000001;

  constexpr auto CODE_TAG_REQUEST = 0x00000000;
  inline bool CODE_TAG_IS_REQUEST(auto code) { return code & CODE_TAG_REQUEST; }
  constexpr auto CODE_TAG_RESPONSE = 0x80000000;
  inline bool CODE_TAG_IS_RESPONSE(auto code) { return code & CODE_TAG_RESPONSE; }

  constexpr auto FULL  = 1 << 31;
  constexpr auto EMPTY = 1 << 30;

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
  constexpr auto PROPERTY_TAG_END = 0x00000000;

  constexpr auto CLOCK_ID_RESERVED  = 0x000000000; // reserved
  constexpr auto CLOCK_ID_EMMC      = 0x000000001; // EMMC
  constexpr auto CLOCK_ID_UART      = 0x000000002; // UART
  constexpr auto CLOCK_ID_ARM       = 0x000000003; // ARM
  constexpr auto CLOCK_ID_CORE      = 0x000000004; // CORE
  constexpr auto CLOCK_ID_V3D       = 0x000000005; // V3D
  constexpr auto CLOCK_ID_H264      = 0x000000006; // H264
  constexpr auto CLOCK_ID_ISP       = 0x000000007; // ISP
  constexpr auto CLOCK_ID_SDRAM     = 0x000000008; // SDRAM
  constexpr auto CLOCK_ID_PIXEL     = 0x000000009; // PIXEL
  constexpr auto CLOCK_ID_PWM       = 0x00000000a; // PWM
  constexpr auto CLOCK_ID_HEVC      = 0x00000000b; // HEVC
  constexpr auto CLOCK_ID_EMMC2     = 0x00000000c; // EMMC2
  constexpr auto CLOCK_ID_M2MC      = 0x00000000d; // M2MC
  constexpr auto CLOCK_ID_PIXEL_BVB = 0x00000000e; // PIXEL_BVB

  constexpr auto PROPERTY_TAG_SET_CLOCK_RATE = 0x00038002;

  constexpr auto MBOX_REQUEST = 0;

  /* tags */
  constexpr auto MBOX_TAG_GETSERIAL  = 0x10004;
  constexpr auto MBOX_TAG_SETCLKRATE = 0x38002;
  constexpr auto MBOX_TAG_LAST       = 0;

  enum class Channel : int {
    POWER_MANAGEMENT                = 0,
    FRAMEBUFFER                     = 1,
    VIRTUAL_UART                    = 2,
    VCHIQ                           = 3,
    LEDS                            = 4,
    BUTTONS                         = 5,
    TOUCH_SCREEN                    = 6,
    COUNT                           = 7,
    PROPERTY_TAGS_ARM_TO_VIDEO_CORE = 8,
    PROPERTY_TAGS_VIDEO_CORE_TO_ARM = 9,
  };

  namespace _property {
    class _SetClockRate {
    public:
      const u32 tag         = PROPERTY_TAG_SET_CLOCK_RATE;
      const u32 buffer_size = 12;
      const u32 tag_code    = CODE_REQUEST;
      const u32 clock_id    = CLOCK_ID_UART;
      const u32 rate;               // =4000000;
      const u32 skip_setting_turbo; // =0;
      _SetClockRate(const u32 rate, const u32 skip_setting_turbo)
          : rate(rate), skip_setting_turbo(skip_setting_turbo) {}

      _SetClockRate(const u32 tag, const u32 buffer_size, const u32 tag_code, const u32 clock_id, const u32 rate,
                    const u32 skip_setting_turbo)
          : tag(tag), buffer_size(buffer_size), tag_code(tag_code), clock_id(clock_id), rate(rate),
            skip_setting_turbo(skip_setting_turbo) {}
    };

  } // namespace _property

  namespace property {
    template <typename T>
    requires std::is_class_v<T>
    struct Property {
    public:
      const u32 size; /*= sizeof(MailboxProperty); */ // todo: fix size, size is 16 aligned
      const u32 code = CODE_REQUEST;

      const T p; // properties

      const u32 end = PROPERTY_TAG_END;

      Property(const u32 size, const T& p) : size(size), p(p) {}

      Property(const u32 size, const u32 code, const T& p, const u32 an_end)
          : size(size), code(code), p(p), end(an_end) {}
    };

    class SetClockRate : public Property<_property::_SetClockRate> {
    public:
      SetClockRate(const u32 rate, const u32 skip_setting_turbo) : Property(36, {rate, skip_setting_turbo}) {}
    };
    static_assert(sizeof(SetClockRate) == 36);
  } // namespace property

  // find a way to put this function to mailbox.cc file
  template <typename P> [[nodiscard]] extern bool call(const Channel& channel, const property::Property<P>& property) {
    do {
      asm volatile("nop"); // todo: magic, remove this line will cause print to stuck
    } while (*MAILBOX0_STATUS & FULL);
    // send message to mailbox
    u32 x = (underlying_value(channel) & 0xF) | (force_cast(u32, force_cast(unsigned long, &property)) & ~0xf);
    *MAILBOX0_WRITE = x;
    // todo: return true???
    return true;
  }
} // namespace NS_rpi3::NS_mailbox
