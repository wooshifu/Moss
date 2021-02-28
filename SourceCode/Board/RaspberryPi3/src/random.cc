#include "libcxx/log.hh"      // for log_d
#include "rpi3/mmio.hh"       // for MMIO_BASE
#include "rpi3/namespaces.hh" // for NAMESPACE_RPI3
#include "rpi3/random.hh"     // for generate_random, init_random_generator

namespace NS_RPI3 {
  unsigned int* RANDOM_GEN_CTRL       = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104000);
  unsigned int* RANDOM_GEN_STATUS     = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104004);
  const unsigned int* RANDOM_GEN_DATA = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104008);
  unsigned int* RANDOM_GEN_INT_MASK   = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104010);

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

} // namespace NS_RPI3

unsigned int generate_random(unsigned int min, unsigned int max) { return NS_RPI3::generate_random(min, max); }
