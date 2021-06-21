#pragma once

#ifndef __ASSEMBLER__
constexpr unsigned long operator"" _K(unsigned long long K) { return K * 1024; }
constexpr unsigned long operator"" _KB(unsigned long long KB) { return operator""_K(KB); }
constexpr unsigned long operator"" _M(unsigned long long M) { return M * 1024 * 1024; }
constexpr unsigned long operator"" _MB(unsigned long long MB) { return operator""_M(MB); }
constexpr unsigned long operator"" _G(unsigned long long G) { return G * 1024 * 1024 * 1024; }
constexpr unsigned long operator"" _GB(unsigned long long GB) { return operator""_G(GB); }
#endif
