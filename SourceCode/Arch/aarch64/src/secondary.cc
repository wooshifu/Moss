// todo: not implemented
extern "C" [[noreturn]] void arm64_secondary_entry() {
  while (true) {
    asm volatile("wfe");
  }
}
