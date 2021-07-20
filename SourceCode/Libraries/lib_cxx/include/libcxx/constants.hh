#pragma once

consteval unsigned int operator"" _Kb(unsigned long long kb) { return kb * 1024; }

/* compile-time function to get the "base" filename, that is, the part of
 a filename after the last "/" or "\" path separator.  The search starts at
 the end of the string; the second parameter is the length of the string. */
consteval const char* compile_time_filename(const char* file_path, const int offset) {
  bool condition = offset == 0 or file_path[offset - 1] == '/' or file_path[offset - 1] == '\\';
  return condition ? file_path + offset : compile_time_filename(file_path, offset - 1);
}
