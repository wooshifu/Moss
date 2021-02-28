#include "libcxx/log.hh"      // for log_d
#include "rpi3/namespaces.hh" // for NAMESPACE_RPI3_BEGIN, NAMESPACE_RPI3_END
#include "rpi3/random.hh"     // for RANDOM_GEN_STATUS, RANDOM_GEN_CTRL

NAMESPACE_RPI3_BEGIN

void init_random_generator() {
  *RANDOM_GEN_STATUS = 0x40000;
  // mask interrupt
  *RANDOM_GEN_INT_MASK |= 1;
  // enable
  *RANDOM_GEN_CTRL |= 1;
  // wait for gaining some entropy
  while (!((*RANDOM_GEN_STATUS) >> 24)) {
    log_d("init_random_generator");
    asm volatile("nop");
  }
}

unsigned int generate_random(unsigned int min, unsigned int max) { return *RANDOM_GEN_DATA % (max - min) + min; }

NAMESPACE_RPI3_END
