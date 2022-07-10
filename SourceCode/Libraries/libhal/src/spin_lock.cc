#include "libhal/spin_lock.hh"

void spin_lock(void* lock) { arch_spin_lock(lock); }

void spin_unlock(void* lock) { arch_spin_unlock(lock); }
