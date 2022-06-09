#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "Simplify"

#pragma once

consteval auto MMU_INITIAL_MAPPING_PHYS_OFFSET() {
  if (IS_64BIT) { return 0; }
  return 0;
}

consteval auto MMU_INITIAL_MAPPING_VIRT_OFFSET() {
  if (IS_64BIT) { return 8; }
  return 4;
}

consteval auto MMU_INITIAL_MAPPING_SIZE_OFFSET() {
  if (IS_64BIT) return 16;
  return 8;
}
consteval auto MMU_INITIAL_MAPPING_FLAGS_OFFSET() {
  if (IS_64BIT) return 24;
  return 12;
}
consteval auto MMU_INITIAL_MAPPING_SIZE() {
  if (IS_64BIT) return 40;
  return 20;
}

/* flags for initial mapping struct */
constexpr auto MMU_INITIAL_MAPPING_TEMPORARY     = 0x1;
constexpr auto MMU_INITIAL_MAPPING_FLAG_UNCACHED = 0x2;
constexpr auto MMU_INITIAL_MAPPING_FLAG_DEVICE   = 0x4;
constexpr auto MMU_INITIAL_MAPPING_FLAG_DYNAMIC  = 0x8; /* entry has to be patched up by platform_reset */

#pragma clang diagnostic pop
