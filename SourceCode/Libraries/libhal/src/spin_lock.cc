#include "libhal/spin_lock.hh"

void spin_lock(spin_lock_t& lock) { arch_spin_lock(lock); }

void spin_unlock(spin_lock_t& lock) { arch_spin_unlock(lock); }
