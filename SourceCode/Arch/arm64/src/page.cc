#include "arm64/mm.hpp"
#include "arm64/page.hpp"
#include "arm64/pgtable_hwdef.hpp"
#include "libcxx/log.hpp"

#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

static unsigned short MEMORY_MAP[NR_PAGES] = {
    0,
};

static unsigned long PHYSIC_START_ADDRESS;

void memInit(unsigned long startMemory, unsigned long endMemory) {
  unsigned long freePages = 0;

  startMemory          = PAGE_ALIGN(startMemory);
  PHYSIC_START_ADDRESS = startMemory;
  endMemory &= PAGE_MASK;
  unsigned long free = endMemory - startMemory;

  while (startMemory < endMemory) {
    freePages++;
    startMemory += PAGE_SIZE;
  }

  log_i("Memory: %uKB available, %u free pages\n", free / 1024, freePages);
}

#define LOW_MEMORY (2 * SECTION_SIZE)
unsigned long getFreePage() {
  for (size_t i = 0; i < NR_PAGES; i++) {
    if (MEMORY_MAP[i] == 0) {
      MEMORY_MAP[i] = 1;
      return LOW_MEMORY + i * PAGE_SIZE;
    }
  }
  return 0;
}

void freePage(unsigned long p) { MEMORY_MAP[(p - LOW_MEMORY) / PAGE_SIZE] = 0; }
