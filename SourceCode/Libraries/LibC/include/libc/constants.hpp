#pragma once

constexpr unsigned int _1024 = 1024;
constexpr unsigned int operator"" _Kb(unsigned long long kb) { return kb * _1024; }
