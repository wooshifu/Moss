#pragma once

#include "libcxx/macros.hh" // for noncopyable, nonmovable

void arch_spin_lock(void* lock);
void arch_spin_unlock(void* lock);

void spin_lock(void* lock);
void spin_unlock(void* lock);

class auto_spin_lock {
public:
  explicit auto_spin_lock(void* lock) : _lock(lock) { spin_lock(_lock); }
  ~auto_spin_lock() { spin_unlock(_lock); }

  noncopyable(auto_spin_lock);
  nonmovable(auto_spin_lock);

private:
  void* _lock;
};
