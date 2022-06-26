#include "hal/io.hh"       // for platform_dputc
#include "libcxx/attr.hh"  // for attr_unused
#include "libcxx/types.hh" // for size_t, ssize_t
#include "libio/io.hh"     // for io_handle_t, IO_HANDLE_MAGIC, io_handle_h...

enum {
  /* Possible future flags:
   * SPIN_LOCK_FLAG_PMR_MASK         = 0x000000ff,
   * SPIN_LOCK_FLAG_PREEMPTION       = 0x10000000,
   * SPIN_LOCK_FLAG_SET_PMR          = 0x20000000,
   */

  /* ARM specific flags */
  SPIN_LOCK_FLAG_IRQ        = 0x40000000,
  SPIN_LOCK_FLAG_FIQ        = 0x80000000, /* Do not use unless IRQs are already disabled */
  SPIN_LOCK_FLAG_IRQ_FIQ    = SPIN_LOCK_FLAG_IRQ | SPIN_LOCK_FLAG_FIQ,

  /* Generic flags */
  SPIN_LOCK_FLAG_INTERRUPTS = SPIN_LOCK_FLAG_IRQ,
};

/* default arm flag is to just disable plain irqs */
#define ARCH_DEFAULT_SPIN_LOCK_FLAG_INTERRUPTS SPIN_LOCK_FLAG_INTERRUPTS

/* Generic flags */
#define SPIN_LOCK_FLAG_INTERRUPTS              ARCH_DEFAULT_SPIN_LOCK_FLAG_INTERRUPTS

#define PRINT_LOCK_FLAGS                       SPIN_LOCK_FLAG_INTERRUPTS

// static spin_lock_t print_spin_lock = 0;
// static list_node print_callbacks   = LIST_INITIAL_VALUE(print_callbacks);

/* print lock must be held when invoking out, outs, outc */
static void out_count(const char* str, size_t len) {
#if 0
  print_callback_t* cb = nullptr;

  /* print to any registered loggers */
  if (!list_is_empty(&print_callbacks)) {
    spin_lock_saved_state_t state{};
    spin_lock_save(&print_spin_lock, &state, PRINT_LOCK_FLAGS);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
    list_for_every_entry(&print_callbacks, cb, print_callback_t, entry) {
      if (cb->print != nullptr) {
        cb->print(cb, str, len);
      }
    }

    spin_unlock_restore(&print_spin_lock, state, PRINT_LOCK_FLAGS);
  }
#endif

  /* write out the serial port */
  for (size_t i = 0; i < len; i++) {
    platform_dputc(str[i]);
  }
}

static ssize_t __debug_stdio_write(attr_unused io_handle_t* io, const char* str, size_t len) {
  out_count(str, len);
  return static_cast<ssize_t>(len);
}

static ssize_t __debug_stdio_read(io_handle_t* io, char* str, size_t len) { // NOLINT(readability-non-const-parameter)
  (void)(io);
  (void)(str);
  (void)(len);
  // todo:
  return 0;
}

/* global console io handle */
static const io_handle_hooks_t console_io_hooks = {
    .write = __debug_stdio_write,
    .read  = __debug_stdio_read,
};

io_handle_t console_io = {.magic = (IO_HANDLE_MAGIC), .hooks = &console_io_hooks};
