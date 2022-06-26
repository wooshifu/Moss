#include "bcm28xx/bcm28xx.hh" // for MINIUART_BASE
#include "hal/uart.hh"        // for uart_putc
#include "libcxx/attr.hh"     // for attr_unused
#include "libcxx/types.hh"    // for uint32_t, uintptr_t

struct bcm283x_mu_regs {
  u32 io;
  u32 ier;
  u32 iir;
  u32 lcr;
  u32 mcr;
  u32 lsr;
  u32 msr;
  u32 scratch;
  u32 cntl;
  u32 stat;
  u32 baud;
};

static inline volatile u32* REG32(auto addr) { return (volatile u32*)(uintptr_t)(addr); }
static inline void write32(auto value, auto addr) { *REG32(addr) = value; }
static inline auto read32(auto addr) -> u32 { return *REG32(addr); }
constexpr auto MU_LSR_TX_EMPTY = 1 << 5;

int uart_putc(attr_unused int port, char ch) {
  // There's only one UART for now.
  auto* regs = reinterpret_cast<bcm283x_mu_regs*>(MINIUART_BASE);

  // Wait until there is space in the FIFO
  while ((read32(&regs->lsr) & MU_LSR_TX_EMPTY) == 0U) {
  }

  // Send the character
  write32(ch, &regs->io);

  return 1;
}
