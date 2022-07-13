#pragma once

#include "libcxx/macros.hh" // for noncopyable, nonmovable
#include "libcxx/types.hh"  // for u64

struct spin_lock_t {
  volatile u64 lock;
};

void arch_spin_lock(spin_lock_t& lock);
void arch_spin_unlock(spin_lock_t& lock);

void spin_lock(spin_lock_t& lock);
void spin_unlock(spin_lock_t& lock);

class auto_spin_lock {
public:
  explicit auto_spin_lock(spin_lock_t& lock) : _lock(lock) { spin_lock(_lock); }
  ~auto_spin_lock() { spin_unlock(_lock); }

  noncopyable(auto_spin_lock);
  nonmovable(auto_spin_lock);

private:
  spin_lock_t& _lock;
};
