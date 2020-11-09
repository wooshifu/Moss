#pragma once

void abort();

void exit(int exit_code);

int atexit(void (*func)());

constexpr int EXIT_SUCCESS = 0;
constexpr int EXIT_FAILURE = -1;
