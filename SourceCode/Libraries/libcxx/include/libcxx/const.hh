#pragma once

#include "libcxx/compiler.hh"
#include "libcxx/macros.hh"

consteval auto last_nbits(auto n) {
  int count = 0;
  for (int i = 0; i < sizeof(n) << 3; ++i) {
    if (n & (0x1UL << i)) {
      count++;
    } else {
      return count;
    }
  }
  return count;
}
static_test(last_nbits(0b110111) == 3);
static_test(last_nbits(0b11011000000) == 0);
static_test(last_nbits(0b1101100100) == 0);
static_test(last_nbits(0x0000'ffff'ffff'ffff) == 48);

// from left to right, starting with 0
consteval auto first_bit_index(auto n) {
  if (n == 0) { trigger_consteval_failure("n should not be 0"); }

  for (int i = (sizeof(n) << 3) - 1; i >= 0; --i) {
    if (n & (0x1ULL << i)) { return i; }
  }

  trigger_consteval_failure("first bit index not found");
  return 0;
}
static_test(first_bit_index(0b1) == 0);
static_test(first_bit_index(0b110110) == 5);
static_test(first_bit_index(0b110'1100'0000) == 10);
static_test(first_bit_index(0b10110'1100'0000) == 12);
static_test(first_bit_index(0x0800'0000'0000) == 43);
static_test(first_bit_index(0x8000'0000'0000'0000) == 63);

consteval auto first_nbits(auto n) {
  if (n == 0) { return 0; }

  int first_bit_index = 0;
  for (int i = (sizeof(n) << 3) - 1; i >= 0; --i) {
    if (n & (0x1ULL << i)) {
      first_bit_index = i;
      break;
    }
  }

  int count = 0;
  for (int i = first_bit_index; i >= 0; i--) {
    if (n & (0x1ULL << i)) {
      count++;
    } else {
      break;
    }
  }
  return count;
}
static_test(first_nbits(0b0) == 0);
static_test(first_nbits(0b1) == 1);
static_test(first_nbits(0b10) == 1);
static_test(first_nbits(0b1111000) == 4);
static_test(first_nbits(0b111100011) == 4);
static_test(first_nbits(0b1111000110) == 4);
static_test(first_nbits(0xffff'0000'0000'0000) == 16);

consteval auto count_bit(auto n) {
  int count = 0;
  for (int i = 0; i < sizeof(n) << 3; ++i) {
    if (n & (0x1UL << i)) { count++; }
  }
  return count;
}

static_test(count_bit(0x0) == 0);
static_test(count_bit(0x1) == 1);
static_test(count_bit(0b10001) == 2);
static_test(count_bit(0b1000'1010) == 3);
static_test(count_bit(0xffff'ffff'ffff'ffff) == 64);
static_test(count_bit(0x0000'ffff'ffff'ffff) == 48);
