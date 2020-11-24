#pragma once

#ifndef __STRINGIFY__
#define __INTERNAL_STRINGIFY_HELPER__(x) #x
#define __STRINGIFY__(x)                 __INTERNAL_STRINGIFY_HELPER__(x)
#endif

#ifndef extern_C
#define extern_C extern "C"
#endif

#if 0
/* compile-time function to get the "base" filename, that is, the part of
 a filename after the last "/" or "\" path separator.  The search starts at
 the end of the string; the second parameter is the length of the string. */
constexpr const char *compile_time_filename(const char *file_path, const int offset) {
  bool condition = offset == 0 or file_path[offset - 1] == '/' or file_path[offset - 1] == '\\';
  return condition ? file_path + offset : compile_time_filename(file_path, offset - 1);
}

#ifndef __CURRENT_FILE_NAME__
#define __CURRENT_FILE_NAME__ compile_time_filename(__FILE__, sizeof(__FILE__) - 1)
#endif

#ifndef __CURRENT_FILE_PATH_LINE__
#define __CURRENT_FILE_PATH_LINE__ __FILE__ ":" __STRINGIFY__(__LINE__)
#endif

#ifndef __CURRENT_FILE_LINE__
#define __CURRENT_FILE_LINE__ compile_time_filename(__CURRENT_FILE_PATH_LINE__, sizeof(__CURRENT_FILE_PATH_LINE__) - 1)
#endif
#endif
