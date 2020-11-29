#include "arm64/mm.hpp"
#include "arm64/page.hpp"
#include "arm64/pgtable_hwdef.hpp"
#include "libcxx/log.hpp"

constexpr auto NR_PAGES = (TOTAL_MEMORY / PAGE_SIZE);

static unsigned short mem_map[NR_PAGES] = {
    0,
};

static unsigned long phy_start_address;

void mem_init(unsigned long start_mem, unsigned long end_mem) {
  unsigned long nr_free_pages = 0;

  start_mem         = PAGE_ALIGN(start_mem);
  phy_start_address = start_mem;
  end_mem &= PAGE_MASK;
  unsigned long free = end_mem - start_mem;

  while (start_mem < end_mem) {
    nr_free_pages++;
    start_mem += PAGE_SIZE;
  }

  log_i("Memory: %uKB available, %u free pages\n", free / 1024, nr_free_pages);
}

constexpr auto LOW_MEMORY = (2 * SECTION_SIZE);
unsigned long get_free_page() {
  for (size_t i = 0; i < NR_PAGES; i++) {
    if (mem_map[i] == 0) {
      mem_map[i] = 1;
      return LOW_MEMORY + i * PAGE_SIZE;
    }
  }
  return 0;
}

void free_page(unsigned long p) { mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0; }
