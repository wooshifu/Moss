#pragma once

#include "libcxx/cast.hh"
#include "rpi3/namespaces.hh"

namespace NS_RPI3 {
  constexpr int MMIO_BASE = 0x3F000000;

  // GPIO Function Select
  static volatile unsigned int* GPFSEL0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200000);
  static volatile unsigned int* GPFSEL1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200004);
  static volatile unsigned int* GPFSEL2 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200008);
  static volatile unsigned int* GPFSEL3 = force_cast(volatile unsigned int*, MMIO_BASE + 0x0020000C);
  static volatile unsigned int* GPFSEL4 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200010);
  static volatile unsigned int* GPFSEL5 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200014);
  // GPIO Pin Output Set
  static volatile unsigned int* GPSET0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x0020001C);
  static volatile unsigned int* GPSET1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200020);
  // GPIO Pin Output Clear
  static volatile unsigned int* GPCLR0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200028);
  // GPIO Pin Level
  static volatile unsigned int* GPLEV0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200034);
  static volatile unsigned int* GPLEV1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200038);
  // GPIO Pin Event Detect Status
  static volatile unsigned int* GPEDS0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200040);
  static volatile unsigned int* GPEDS1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200044);
  // GPIO Pin High Detect Enable
  static volatile unsigned int* GPHEN0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200064);
  static volatile unsigned int* GPHEN1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200068);
  // Pin Pull-up/down Enable
  // see https://docs.huihoo.com/doxygen/linux/kernel/3.7/pinctrl-bcm2835_8c_source.html#L66
  static volatile unsigned int* GPPUD = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200094);
  // Pin Pull-up/down Enable Clock
  // see https://docs.huihoo.com/doxygen/linux/kernel/3.7/pinctrl-bcm2835_8c_source.html #L67
  static volatile unsigned int* GPPUDCLK0 = force_cast(volatile unsigned int*, MMIO_BASE + 0x00200098);
  static volatile unsigned int* GPPUDCLK1 = force_cast(volatile unsigned int*, MMIO_BASE + 0x0020009C);

  constexpr auto IRQ_BASIC_PENDING  = MMIO_BASE + 0x0000B200;
  constexpr auto IRQ_PENDING_1      = MMIO_BASE + 0x0000B204;
  constexpr auto IRQ_PENDING_2      = MMIO_BASE + 0x0000B208;
  constexpr auto FIQ_CONTROL        = MMIO_BASE + 0x0000B20C;
  constexpr auto ENABLE_IRQS_1      = MMIO_BASE + 0x0000B210;
  constexpr auto ENABLE_IRQS_2      = MMIO_BASE + 0x0000B214;
  constexpr auto ENABLE_BASIC_IRQS  = MMIO_BASE + 0x0000B218;
  constexpr auto DISABLE_IRQS_1     = MMIO_BASE + 0x0000B21C;
  constexpr auto DISABLE_IRQS_2     = MMIO_BASE + 0x0000B220;
  constexpr auto DISABLE_BASIC_IRQS = MMIO_BASE + 0x0000B224;

  constexpr auto SYSTEM_TIMER_IRQ_0 = 1 << 0;
  constexpr auto SYSTEM_TIMER_IRQ_1 = 1 << 1;
  constexpr auto SYSTEM_TIMER_IRQ_2 = 1 << 2;
  constexpr auto SYSTEM_TIMER_IRQ_3 = 1 << 3;

  // See BCM2836 ARM-local peripherals at
  // https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf
  constexpr auto LOCAL_PERIPHERALS_BASE = 0x40000000;

  constexpr auto LOCAL_TIMER_CONTROL                        = LOCAL_PERIPHERALS_BASE + 0x34;
  constexpr auto LOCAL_TIMER_CLEAR                          = LOCAL_PERIPHERALS_BASE + 0x38;
  constexpr auto LOCAL_INTERRUPT_ROUTING                    = LOCAL_PERIPHERALS_BASE + 0x24;
  constexpr auto LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ = 0b000;
  constexpr auto COREn_LOCAL_TIMER_INTERRUPT_SOURCE         = 1 << 11;

  constexpr auto LOCAL_TIMER_CONTROL_ENABLED           = 1 << 28;
  constexpr auto LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED = 1 << 29;
  constexpr auto LOCAL_TIMER_CONTROL_VALUE = LOCAL_TIMER_CONTROL_ENABLED | LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED;
  constexpr auto LOCAL_TIMER_CLEAR_ACK     = 1 << 31;

  constexpr auto CORE0_TIMER_INTERRUPT_CONTROL    = LOCAL_PERIPHERALS_BASE + 0x40;
  constexpr auto CORE0_INTERRUPT_SOURCES          = LOCAL_PERIPHERALS_BASE + 0x60;
  constexpr auto COREn_CNTV_IRQ_INTERRUPT_ENABLED = 1 << 3;
  constexpr auto COREn_CNTV_INTERRUPT_SOURCE      = 1 << 3;
} // namespace NS_RPI3
