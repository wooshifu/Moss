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
