#pragma once

void abort();

void exit(int exit_code);

int atexit(void (*func)(void));

#define EXIT_SUCCESS 0
#define EXIT_FAILURE (-1)
