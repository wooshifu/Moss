#pragma once

#define MMIO_BASE 0x3F000000

// GPIO Function Select
#define GPFSEL0 ((volatile unsigned int *)(MMIO_BASE + 0x00200000))
#define GPFSEL1 ((volatile unsigned int *)(MMIO_BASE + 0x00200004))
#define GPFSEL2 ((volatile unsigned int *)(MMIO_BASE + 0x00200008))
#define GPFSEL3 ((volatile unsigned int *)(MMIO_BASE + 0x0020000C))
#define GPFSEL4 ((volatile unsigned int *)(MMIO_BASE + 0x00200010))
#define GPFSEL5 ((volatile unsigned int *)(MMIO_BASE + 0x00200014))
// GPIO Pin Output Set
#define GPSET0 ((volatile unsigned int *)(MMIO_BASE + 0x0020001C))
#define GPSET1 ((volatile unsigned int *)(MMIO_BASE + 0x00200020))
// GPIO Pin Output Clear
#define GPCLR0 ((volatile unsigned int *)(MMIO_BASE + 0x00200028))
// GPIO Pin Level
#define GPLEV0 ((volatile unsigned int *)(MMIO_BASE + 0x00200034))
#define GPLEV1 ((volatile unsigned int *)(MMIO_BASE + 0x00200038))
// GPIO Pin Event Detect Status
#define GPEDS0 ((volatile unsigned int *)(MMIO_BASE + 0x00200040))
#define GPEDS1 ((volatile unsigned int *)(MMIO_BASE + 0x00200044))
// GPIO Pin High Detect Enable
#define GPHEN0 ((volatile unsigned int *)(MMIO_BASE + 0x00200064))
#define GPHEN1 ((volatile unsigned int *)(MMIO_BASE + 0x00200068))
// Pin Pull-up/down Enable
// see https://docs.huihoo.com/doxygen/linux/kernel/3.7/pinctrl-bcm2835_8c_source.html#L66
#define GPPUD ((volatile unsigned int *)(MMIO_BASE + 0x00200094))
// Pin Pull-up/down Enable Clock
// see https://docs.huihoo.com/doxygen/linux/kernel/3.7/pinctrl-bcm2835_8c_source.html #L67
#define GPPUDCLK0 ((volatile unsigned int *)(MMIO_BASE + 0x00200098))
#define GPPUDCLK1 ((volatile unsigned int *)(MMIO_BASE + 0x0020009C))

#define IRQ_BASIC_PENDING (MMIO_BASE + 0x0000B200)
#define IRQ_PENDING_1 (MMIO_BASE + 0x0000B204)
#define IRQ_PENDING_2 (MMIO_BASE + 0x0000B208)
#define FIQ_CONTROL (MMIO_BASE + 0x0000B20C)
#define ENABLE_IRQS_1 (MMIO_BASE + 0x0000B210)
#define ENABLE_IRQS_2 (MMIO_BASE + 0x0000B214)
#define ENABLE_BASIC_IRQS (MMIO_BASE + 0x0000B218)
#define DISABLE_IRQS_1 (MMIO_BASE + 0x0000B21C)
#define DISABLE_IRQS_2 (MMIO_BASE + 0x0000B220)
#define DISABLE_BASIC_IRQS (MMIO_BASE + 0x0000B224)

#define SYSTEM_TIMER_IRQ_0 (1 << 0)
#define SYSTEM_TIMER_IRQ_1 (1 << 1)
#define SYSTEM_TIMER_IRQ_2 (1 << 2)
#define SYSTEM_TIMER_IRQ_3 (1 << 3)

// See BCM2836 ARM-local peripherals at
// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf

#define LOCAL_PERIPHERALS_BASE 0x40000000
#define LOCAL_TIMER_INTERRUPT (1 << 11)

#define LOCAL_TIMER_CONTROL (LOCAL_PERIPHERALS_BASE + 0x34)
#define LOCAL_TIMER_CLEAR (LOCAL_PERIPHERALS_BASE + 0x38)
#define LOCAL_INTERRUPT_ROUTING (LOCAL_PERIPHERALS_BASE + 0x24)
#define LOCAL_TIMER_INTERRUPT_ROUTING_TO_CORE0_IRQ 0b000

#define LOCAL_TIMER_CONTROL_ENABLED (1 << 28)
#define LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED (1 << 29)
#define LOCAL_TIMER_CONTROL_VALUE (LOCAL_TIMER_CONTROL_ENABLED | LOCAL_TIMER_CONTROL_INTERRUPT_ENABLED)
#define LOCAL_TIMER_CLEAR_ACK (1 << 31)

#define CORE0_TIMER_INTERRUPT_CONTROL (LOCAL_PERIPHERALS_BASE + 0x40)
#define CORE0_INTERRUPT_SOURCES (LOCAL_PERIPHERALS_BASE + 0x60)
#define COREn_CNTV_IRQ_INTERRUPT_ENABLED (1 << 3)
