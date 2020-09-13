#include "libc/memory.h"
#include "rpi3/interrupt_controller.h"
#include "rpi3/mmio.h"

/**
 * THIS IS UNUSED INTERRUPT CONTROLLER
 *
 * see more from here:
 * https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson03/rpi-os.md#configuring-interrupt-controller
 */
void enable_interrupt_controller() { memory_write_32bits((uint32_t *)ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1); }
