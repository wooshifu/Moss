#pragma once

#include <stddef.h>
#include <stdint.h>

void memzero(uint8_t *mem_start, const uint8_t *mem_end);

void *memset(void *destination, int character, size_t count);

size_t strlen(const char *str);

int strcmp(const char *left, const char *right);

char *strchr(const char *str, int character);

char *strrchr(const char *str, int character);
