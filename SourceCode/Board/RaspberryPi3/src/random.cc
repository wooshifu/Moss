#include "libcxx/cast.hh" // for force_cast
#include "libcxx/log.hh"  // for log_d
#include "rpi3/mmio.hh"   // for NS_mmio
#include "rpi3/random.hh" // for generate_random, init_random_generator
#include "rpi3/rpi3.hh"   // for NS_RPI3

namespace NS_rpi3 {
  unsigned int* RANDOM_GEN_CTRL       = force_cast(unsigned int*, NS_mmio::BASE + 0x00104000);
  unsigned int* RANDOM_GEN_STATUS     = force_cast(unsigned int*, NS_mmio::BASE + 0x00104004);
  const unsigned int* RANDOM_GEN_DATA = force_cast(const unsigned int*, NS_mmio::BASE + 0x00104008);
  unsigned int* RANDOM_GEN_INT_MASK   = force_cast(unsigned int*, NS_mmio::BASE + 0x00104010);

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
} // namespace NS_rpi3

unsigned int generate_random(unsigned int min, unsigned int max) { return NS_rpi3::generate_random(min, max); }
