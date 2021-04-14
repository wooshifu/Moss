enum reboot_flags {
  REBOOT_NORMAL = 0,
  REBOOT_BOOTLOADER = 1,
  REBOOT_RECOVERY = 2,
};

extern "C" void psci_system_reset(enum reboot_flags flags) {
}