#pragma once

#include "libcxx/macros.hh"

#if ARCH_AARCH64
using spin_lock_t             = unsigned long;

using spin_lock_saved_state_t = unsigned int;
using spin_lock_save_flags_t  = unsigned int;

extern_C {
  void arch_spin_lock(spin_lock_t * lock);
  int arch_spin_trylock(spin_lock_t * lock);
  void arch_spin_unlock(spin_lock_t * lock);
  void arch_interrupt_save(spin_lock_saved_state_t * statep, spin_lock_save_flags_t flags);
  void arch_interrupt_restore(spin_lock_saved_state_t * statep, spin_lock_save_flags_t flags);
}
#endif

/* interrupts should already be disabled */
static inline void spin_lock(spin_lock_t* lock) { arch_spin_lock(lock); }

/* Returns 0 on success, non-0 on failure */
static inline int spin_trylock(spin_lock_t* lock) { return arch_spin_trylock(lock); }

/* interrupts should already be disabled */
static inline void spin_unlock(spin_lock_t* lock) { arch_spin_unlock(lock); }

// static inline void spin_lock_init(spin_lock_t *lock) {
//   arch_spin_lock_init(lock);
// }
//
// static inline bool spin_lock_held(spin_lock_t *lock) {
//   return arch_spin_lock_held(lock);
// }

#if 0
/* spin lock irq save flags: */

/* Possible future flags:
 * SPIN_LOCK_FLAG_PMR_MASK         = 0x000000ff
 * SPIN_LOCK_FLAG_PREEMPTION       = 0x00000100
 * SPIN_LOCK_FLAG_SET_PMR          = 0x00000200
 */

/* Generic flags */
#define SPIN_LOCK_FLAG_INTERRUPTS            ARCH_DEFAULT_SPIN_LOCK_FLAG_INTERRUPTS

/* same as spin lock, but save disable and save interrupt state first */
static inline void spin_lock_save(spin_lock_t* lock, spin_lock_saved_state_t* statep, spin_lock_save_flags_t flags) {
  arch_interrupt_save(statep, flags);
  spin_lock(lock);
}

/* restore interrupt state before unlocking */
static inline void spin_unlock_restore(spin_lock_t* lock, spin_lock_saved_state_t old_state,
                                       spin_lock_save_flags_t flags) {
  spin_unlock(lock);
  arch_interrupt_restore(old_state, flags);
}

/* hand(ier) routines */
#define spin_lock_irqsave(lock, statep)      spin_lock_save(lock, &(statep), SPIN_LOCK_FLAG_INTERRUPTS)
#define spin_unlock_irqrestore(lock, statep) spin_unlock_restore(lock, statep, SPIN_LOCK_FLAG_INTERRUPTS)

#endif
