//
// Created by shifu on 2021/10/17.
//

#pragma once

#if 0
Kilobyte (KB) 	1000^1 = 1,000
Megabyte (MB) 	1000^2 = 1,000,000
Gigabyte (GB) 	1000^3 = 1,000,000,000
Terabyte (TB) 	1000^4 = 1,000,000,000,000
Petabyte (PB) 	1000^5 = 1,000,000,000,000,000

Kibibyte (KiB) 	1024^1 = 1,024
Mebibyte (MiB) 	1024^2 = 1,048,576
Gibibyte (GiB) 	1024^3 = 1,073,741,824
Tebibyte (TiB) 	1024^4 = 1,099,511,627,776
Pebibyte (PiB) 	1024^5 = 1,125,899,906,842,624
#endif

consteval unsigned long operator"" _KB(unsigned long long KB) { return 1000 * KB; }
static_assert(1_KB == 1000);
consteval unsigned long operator"" _KiB(unsigned long long KiB) { return 1024 * KiB; }
static_assert(1_KiB == 1024);
consteval unsigned long operator"" _MB(unsigned long long MB) { return operator""_KB(1000 * MB); }
static_assert(1_MB == 1000'000);
consteval unsigned long operator"" _MiB(unsigned long long MiB) { return operator""_KiB(1024 * MiB); }
static_assert(1_MiB == 1024UL * 1024UL);
consteval unsigned long operator"" _GB(unsigned long long GB) { return operator""_MB(1000 * GB); }
static_assert(1_GB == 1000'000'000);
consteval unsigned long operator"" _GiB(unsigned long long GiB) { return operator""_MiB(1024 * GiB); }
static_assert(1_GiB == 1024UL * 1024UL * 1024UL);
