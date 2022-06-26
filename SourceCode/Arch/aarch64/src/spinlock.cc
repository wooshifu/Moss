void _arch_spinklock_functions() {
  asm volatile(R"(
.include "libstd/asm.hh"

FUNCTION arch_spin_trylock
    mov	    x2, x0
    mov	    x1, #1
    ldaxr   x0, [x2]
    cbnz    x0, 1f
    stxr    w0, x1, [x2]
1:
    ret

FUNCTION arch_spin_lock
    mov     x1, #1
    sevl
1:
    wfe
    ldaxr   x2, [x0]
    cbnz    x2, 1b
    stxr    w2, x1, [x0]
    cbnz    w2, 1b
    ret

FUNCTION arch_spin_unlock
    stlr    xzr, [x0]
    ret

)");
}

#if 0
void arch_interrupt_save(spin_lock_saved_state_t* statep, spin_lock_save_flags_t flags) {
  spin_lock_saved_state_t state = 0;
  if ((flags & SPIN_LOCK_FLAG_IRQ) && !arch_ints_disabled()) {
    state |= SPIN_LOCK_STATE_RESTORE_IRQ;
    arch_disable_ints();
  }
  if ((flags & SPIN_LOCK_FLAG_FIQ) && !arch_fiqs_disabled()) {
    state |= SPIN_LOCK_STATE_RESTORE_FIQ;
    arch_disable_fiqs();
  }
  *statep = state;
}

void arch_interrupt_restore(spin_lock_saved_state_t old_state, spin_lock_save_flags_t flags) {
  if ((flags & SPIN_LOCK_FLAG_FIQ) && (old_state & SPIN_LOCK_STATE_RESTORE_FIQ))
    arch_enable_fiqs();
  if ((flags & SPIN_LOCK_FLAG_IRQ) && (old_state & SPIN_LOCK_STATE_RESTORE_IRQ))
    arch_enable_ints();
}
#endif
