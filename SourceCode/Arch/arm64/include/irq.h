#pragma once

#include "gpio.h"

void irq_vector_init();
void enable_interrupt_controller();

void enable_irq();

void disable_irq();

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

#define LPBASE 0x40000000
#define CORE0_INTERRUPT_SOURCES (LPBASE + 0x60)
#define LTIMER_INTERRUPT (1 << 11)