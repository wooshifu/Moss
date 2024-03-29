#pragma once

#include "libcxx/macro.hh"
#include "libcxx/types.hh"

void memzero(u8* mem_start, const u8* mem_end);
extern_C void* memset(void* destination, int character, size_t count);

size_t strlen(const char* str);
int strcmp(const char* left, const char* right);
char* strchr(const char* str, int character);
char* strrchr(const char* str, int character);
char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t count);
char* strcat(char* destination, const char* source);
char* strncat(char* destination, const char* source, size_t count);
