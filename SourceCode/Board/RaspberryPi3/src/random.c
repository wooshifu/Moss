#include "raspi3/mmio.h"
#include "raspi3/random.h"

#define RANDOM_GEN_CTRL ((volatile unsigned int *)(MMIO_BASE + 0x00104000))
#define RANDOM_GEN_STATUS ((volatile unsigned int *)(MMIO_BASE + 0x00104004))
#define RANDOM_GEN_DATA ((volatile unsigned int *)(MMIO_BASE + 0x00104008))
#define RANDOM_GEN_INT_MASK ((volatile unsigned int *)(MMIO_BASE + 0x00104010))

void init_random_generator() {
  *RANDOM_GEN_STATUS = 0x40000;
  // mask interrupt
  *RANDOM_GEN_INT_MASK |= 1;
  // enable
  *RANDOM_GEN_CTRL |= 1;
  // wait for gaining some entropy
  while (!((*RANDOM_GEN_STATUS) >> 24)) {
    asm volatile("nop");
  }
}

unsigned int generate_random(unsigned int min, unsigned int max) { return *RANDOM_GEN_DATA % (max - min) + min; }
