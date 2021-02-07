#if 0
#include "libcxx/stdarg.hh"
#include "libcxx/string.hh"
#include "libcxx/types.hh"

/* private function */
#define _ISDIGIT(c) ((unsigned)((c) - '0') < 10)
#define RT_PRINTF_LONGLONG
#ifdef RT_PRINTF_LONGLONG
inline int divide(long long* n, int base) {
  int res = 0;

  /* optimized for processor which does not support divide instructions. */
  if (base == 10) {
    res = (int)(((unsigned long long)*n) % 10U);
    *n  = (long long)(((unsigned long long)*n) / 10U);
  } else {
    res = (int)(((unsigned long long)*n) % 16U);
    *n  = (long long)(((unsigned long long)*n) / 16U);
  }

  return res;
}
#else
inline int divide(long* n, int base) {
  int res = 0;

  /* optimized for processor which does not support divide instructions. */
  if (base == 10) {
    res = (int)(((unsigned long)*n) % 10U);
    *n  = (long)(((unsigned long)*n) / 10U);
  } else {
    res = (int)(((unsigned long)*n) % 16U);
    *n  = (long)(((unsigned long)*n) / 16U);
  }

  return res;
}
#endif

inline int skip_atoi(const char** s) {
  int i = 0;
  while (_ISDIGIT(**s)) {
    i = i * 10 + *((*s)++) - '0';
  }

  return i;
}

#define ZEROPAD (1 << 0) /* pad with zero */
#define SIGN    (1 << 1) /* unsigned/signed long */
#define PLUS    (1 << 2) /* show plus */
#define SPACE   (1 << 3) /* space if plus */
#define LEFT    (1 << 4) /* left justified */
#define SPECIAL (1 << 5) /* 0x */
#define LARGE   (1 << 6) /* use 'ABCDEF' instead of 'abcdef' */

#define RT_PRINTF_PRECISION
#ifdef RT_PRINTF_PRECISION
static char* print_number(char* buf, char* end,
#ifdef RT_PRINTF_LONGLONG
                          long long num,
#else
                          long num,
#endif
                          int base, int s, int precision, int type)
#else
static char* print_number(char* buf, char* end,
#ifdef RT_PRINTF_LONGLONG
                          long long num,
#else
                          long num,
#endif
                          int base, int s, int type)
#endif
{
  char c, sign;
#ifdef RT_PRINTF_LONGLONG
  char tmp[32];
#else
  char tmp[16];
#endif
  int precision_bak = precision;
  const char* digits;
  static const char small_digits[] = "0123456789abcdef";
  static const char large_digits[] = "0123456789ABCDEF";
  /*register*/ int i;
  /*register*/ int size;

  size = s;

  digits = (type & LARGE) ? large_digits : small_digits;
  if (type & LEFT)
    type &= ~ZEROPAD;

  c = (type & ZEROPAD) ? '0' : ' ';

  /* get sign */
  sign = 0;
  if (type & SIGN) {
    if (num < 0) {
      sign = '-';
      num  = -num;
    } else if (type & PLUS)
      sign = '+';
    else if (type & SPACE)
      sign = ' ';
  }

#ifdef RT_PRINTF_SPECIAL
  if (type & SPECIAL) {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }
#endif

  i = 0;
  if (num == 0)
    tmp[i++] = '0';
  else {
    while (num != 0)
      tmp[i++] = digits[divide(&num, base)];
  }

#ifdef RT_PRINTF_PRECISION
  if (i > precision)
    precision = i;
  size -= precision;
#else
  size -= i;
#endif

  if (!(type & (ZEROPAD | LEFT))) {
    if ((sign) && (size > 0))
      size--;

    while (size-- > 0) {
      if (buf < end)
        *buf = ' ';
      ++buf;
    }
  }

  if (sign) {
    if (buf < end) {
      *buf = sign;
    }
    --size;
    ++buf;
  }

#ifdef RT_PRINTF_SPECIAL
  if (type & SPECIAL) {
    if (base == 8) {
      if (buf < end)
        *buf = '0';
      ++buf;
    } else if (base == 16) {
      if (buf < end)
        *buf = '0';
      ++buf;
      if (buf < end) {
        *buf = type & LARGE ? 'X' : 'x';
      }
      ++buf;
    }
  }
#endif

  /* no align to the left */
  if (!(type & LEFT)) {
    while (size-- > 0) {
      if (buf < end)
        *buf = c;
      ++buf;
    }
  }

#ifdef RT_PRINTF_PRECISION
  while (i < precision--) {
    if (buf < end)
      *buf = '0';
    ++buf;
  }
#endif

  /* put number in the temporary buffer */
  while (i-- > 0 && (precision_bak != 0)) {
    if (buf < end)
      *buf = tmp[i];
    ++buf;
  }

  while (size-- > 0) {
    if (buf < end)
      *buf = ' ';
    ++buf;
  }

  return buf;
}

int32_t vsnprintf(char* buf, size_t size, const char* fmt, va_list args) {
  // todo: should va_copy(,copied, arg_ptr) here???
#ifdef RT_PRINTF_LONGLONG
  unsigned long long num;
#else
  uint32_t num;
#endif
  int i, len;
  char *str, *end, c;
  const char* s;

  uint8_t base;        /* the base of number */
  uint8_t flags;       /* flags to print number */
  uint8_t qualifier;   /* 'h', 'l', or 'L' for integer fields */
  int32_t field_width; /* width of output field */

#ifdef RT_PRINTF_PRECISION
  int precision; /* min. # of digits for integers and max for a string */
#endif

  str = buf;
  end = buf + size;

  /* Make sure end is always >= buf */
  if (end < buf) {
    end  = ((char*)-1);
    size = end - buf;
  }

  for (; *fmt; ++fmt) {
    if (*fmt != '%') {
      if (str < end)
        *str = *fmt;
      ++str;
      continue;
    }

    /* process flags */
    flags = 0;

    while (1) {
      /* skips the first '%' also */
      ++fmt;
      if (*fmt == '-')
        flags |= LEFT;
      else if (*fmt == '+')
        flags |= PLUS;
      else if (*fmt == ' ')
        flags |= SPACE;
      else if (*fmt == '#')
        flags |= SPECIAL;
      else if (*fmt == '0')
        flags |= ZEROPAD;
      else
        break;
    }

    /* get field width */
    field_width = -1;
    if (_ISDIGIT(*fmt))
      field_width = skip_atoi(&fmt);
    else if (*fmt == '*') {
      ++fmt;
      /* it's the next argument */
      field_width = va_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= LEFT;
      }
    }

#ifdef RT_PRINTF_PRECISION
    /* get the precision */
    precision = -1;
    if (*fmt == '.') {
      ++fmt;
      if (_ISDIGIT(*fmt))
        precision = skip_atoi(&fmt);
      else if (*fmt == '*') {
        ++fmt;
        /* it's the next argument */
        precision = va_arg(args, int);
      }
      if (precision < 0)
        precision = 0;
    }
#endif
    /* get the conversion qualifier */
    qualifier = 0;
#ifdef RT_PRINTF_LONGLONG
    if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
#else
    if (*fmt == 'h' || *fmt == 'l')
#endif
    {
      qualifier = *fmt;
      ++fmt;
#ifdef RT_PRINTF_LONGLONG
      if (qualifier == 'l' && *fmt == 'l') {
        qualifier = 'L';
        ++fmt;
      }
#endif
    }

    /* the default base */
    base = 10;

    switch (*fmt) {
    case 'c':
      if (!(flags & LEFT)) {
        while (--field_width > 0) {
          if (str < end)
            *str = ' ';
          ++str;
        }
      }

      /* get character */
      c = (uint8_t)va_arg(args, int);
      if (str < end)
        *str = c;
      ++str;

      /* put width */
      while (--field_width > 0) {
        if (str < end)
          *str = ' ';
        ++str;
      }
      continue;

    case 's':
      s = va_arg(args, char*);
      if (!s)
        s = "(NULL)";

      len = strlen(s);
#ifdef RT_PRINTF_PRECISION
      if (precision > 0 && len > precision)
        len = precision;
#endif

      if (!(flags & LEFT)) {
        while (len < field_width--) {
          if (str < end)
            *str = ' ';
          ++str;
        }
      }

      for (i = 0; i < len; ++i) {
        if (str < end)
          *str = *s;
        ++str;
        ++s;
      }

      while (len < field_width--) {
        if (str < end)
          *str = ' ';
        ++str;
      }
      continue;

    case 'p':
      if (field_width == -1) {
        field_width = sizeof(void*) << 1;
        flags |= ZEROPAD;
      }
#ifdef RT_PRINTF_PRECISION
      str = print_number(str, end, (long)va_arg(args, void*), 16, field_width, precision, flags);
#else
      str = print_number(str, end, (long)va_arg(args, void*), 16, field_width, flags);
#endif
      continue;

    case '%':
      if (str < end)
        *str = '%';
      ++str;
      continue;

      /* integer number formats - set up the flags and "break" */
    case 'o':
      base = 8;
      break;

    case 'X':
      flags |= LARGE;
    case 'x':
      base = 16;
      break;

    case 'd':
    case 'i':
      flags |= SIGN;
    case 'u':
      break;

    default:
      if (str < end)
        *str = '%';
      ++str;

      if (*fmt) {
        if (str < end)
          *str = *fmt;
        ++str;
      } else {
        --fmt;
      }
      continue;
    }

#ifdef RT_PRINTF_LONGLONG
    if (qualifier == 'L')
      num = va_arg(args, long long);
    else if (qualifier == 'l')
#else
    if (qualifier == 'l')
#endif
    {
      num = va_arg(args, uint32_t);
      if (flags & SIGN)
        num = (int32_t)num;
    } else if (qualifier == 'h') {
      num = (uint16_t)va_arg(args, int32_t);
      if (flags & SIGN)
        num = (int16_t)num;
    } else {
      num = va_arg(args, uint32_t);
      if (flags & SIGN)
        num = (int32_t)num;
    }
#ifdef RT_PRINTF_PRECISION
    str = print_number(str, end, num, base, field_width, precision, flags);
#else
    str = print_number(str, end, num, base, field_width, flags);
#endif
  }

  if (size > 0) {
    if (str < end)
      *str = '\0';
    else {
      end[-1] = '\0';
    }
  }

  /* the trailing null byte doesn't count towards the total
   * ++str;
   */
  return str - buf;
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param size the size of buffer
 * @param fmt the format
 */
int32_t snprintf(char* buf, size_t size, const char* fmt, ...) {
  int32_t n;
  va_list args;

  va_start(args, fmt);
  n = vsnprintf(buf, size, fmt, args);
  va_end(args);

  return n;
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param arg_ptr the arg_ptr
 * @param format the format
 */
int32_t vsprintf(char* buf, const char* format, va_list arg_ptr) {
  va_list va;
  // todo: should va_copy(,copied, arg_ptr) here???
  va_copy(va, arg_ptr);
  auto i = vsnprintf(buf, (size_t)-1, format, va);
  va_end(va);
  return i;
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param format the format
 */
int32_t sprintf(char* buf, const char* format, ...) {
  int32_t n;
  va_list arg_ptr;

  va_start(arg_ptr, format);
  n = vsprintf(buf, format, arg_ptr);
  va_end(arg_ptr);

  return n;
}

void uart_puts(const char* s);
int32_t printf(const char* fmt, ...) {
  constexpr int printf_buf_length = 1024;
  char buf[printf_buf_length]     = {0};

  va_list args;
  va_start(args, fmt);
  //  va_list copied_args;
  // why va_copy???
  //  va_copy(copied_args, args);
  int32_t i = vsprintf(buf, fmt, args);
  va_end(args);
  //  va_end(copied_args);
  uart_puts(buf);
  memset(buf, '\0', printf_buf_length);
  return i;
}

int32_t my_printf(const char* fmt, ...) {
  constexpr int printf_buf_length = 1024;
  char buf[printf_buf_length]     = {0};

  va_list args;
  va_start(args, fmt);
  size_t size       = (size_t)-1;
  const char* fmt_1 = fmt;
//  va_list args;
#ifdef RT_PRINTF_LONGLONG
  unsigned long long num;
#else
  uint32_t num;
#endif
  int i_1, len;
  char *str, *end, c;
  const char* s;

  uint8_t base;        /* the base of number */
  uint8_t flags;       /* flags to print number */
  uint8_t qualifier;   /* 'h', 'l', or 'L' for integer fields */
  int32_t field_width; /* width of output field */

#ifdef RT_PRINTF_PRECISION
  int precision; /* min. # of digits for integers and max for a string */
#endif

  str = buf;
  end = buf + size;

  /* Make sure end is always >= buf */
  if (end < buf) {
    end  = ((char*)-1);
    size = end - buf;
  }

  for (; *fmt_1; ++fmt_1) {
    if (*fmt_1 != '%') {
      if (str < end)
        *str = *fmt_1;
      ++str;
      continue;
    }

    /* process flags */
    flags = 0;

    while (1) {
      /* skips the first '%' also */
      ++fmt_1;
      if (*fmt_1 == '-')
        flags |= LEFT;
      else if (*fmt_1 == '+')
        flags |= PLUS;
      else if (*fmt_1 == ' ')
        flags |= SPACE;
      else if (*fmt_1 == '#')
        flags |= SPECIAL;
      else if (*fmt_1 == '0')
        flags |= ZEROPAD;
      else
        break;
    }

    /* get field width */
    field_width = -1;
    if (_ISDIGIT(*fmt_1))
      field_width = skip_atoi(&fmt_1);
    else if (*fmt_1 == '*') {
      ++fmt_1;
      /* it's the next argument */
      field_width = va_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= LEFT;
      }
    }

#ifdef RT_PRINTF_PRECISION
    /* get the precision */
    precision = -1;
    if (*fmt_1 == '.') {
      ++fmt_1;
      if (_ISDIGIT(*fmt_1))
        precision = skip_atoi(&fmt_1);
      else if (*fmt_1 == '*') {
        ++fmt_1;
        /* it's the next argument */
        precision = va_arg(args, int);
      }
      if (precision < 0)
        precision = 0;
    }
#endif
    /* get the conversion qualifier */
    qualifier = 0;
#ifdef RT_PRINTF_LONGLONG
    if (*fmt_1 == 'h' || *fmt_1 == 'l' || *fmt_1 == 'L')
#else
    if (*fmt == 'h' || *fmt == 'l')
#endif
    {
      qualifier = *fmt_1;
      ++fmt_1;
#ifdef RT_PRINTF_LONGLONG
      if (qualifier == 'l' && *fmt_1 == 'l') {
        qualifier = 'L';
        ++fmt_1;
      }
#endif
    }

    /* the default base */
    base = 10;

    switch (*fmt_1) {
    case 'c':
      if (!(flags & LEFT)) {
        while (--field_width > 0) {
          if (str < end)
            *str = ' ';
          ++str;
        }
      }

      /* get character */
      c = (uint8_t)va_arg(args, int);
      if (str < end)
        *str = c;
      ++str;

      /* put width */
      while (--field_width > 0) {
        if (str < end)
          *str = ' ';
        ++str;
      }
      continue;

    case 's':
      s = va_arg(args, char*);
      if (!s)
        s = "(NULL)";

      len = strlen(s);
#ifdef RT_PRINTF_PRECISION
      if (precision > 0 && len > precision)
        len = precision;
#endif

      if (!(flags & LEFT)) {
        while (len < field_width--) {
          if (str < end)
            *str = ' ';
          ++str;
        }
      }

      for (i_1 = 0; i_1 < len; ++i_1) {
        if (str < end)
          *str = *s;
        ++str;
        ++s;
      }

      while (len < field_width--) {
        if (str < end)
          *str = ' ';
        ++str;
      }
      continue;

    case 'p':
      if (field_width == -1) {
        field_width = sizeof(void*) << 1;
        flags |= ZEROPAD;
      }
#ifdef RT_PRINTF_PRECISION
      str = print_number(str, end, (long)va_arg(args, void*), 16, field_width, precision, flags);
#else
      str = print_number(str, end, (long)va_arg(args, void*), 16, field_width, flags);
#endif
      continue;

    case '%':
      if (str < end)
        *str = '%';
      ++str;
      continue;

      /* integer number formats - set up the flags and "break" */
    case 'o':
      base = 8;
      break;

    case 'X':
      flags |= LARGE;
    case 'x':
      base = 16;
      break;

    case 'd':
    case 'i':
      flags |= SIGN;
    case 'u':
      break;

    default:
      if (str < end)
        *str = '%';
      ++str;

      if (*fmt_1) {
        if (str < end)
          *str = *fmt_1;
        ++str;
      } else {
        --fmt_1;
      }
      continue;
    }

#ifdef RT_PRINTF_LONGLONG
    if (qualifier == 'L')
      num = va_arg(args, long long);
    else if (qualifier == 'l')
#else
    if (qualifier == 'l')
#endif
    {
      num = va_arg(args, uint32_t);
      if (flags & SIGN)
        num = (int32_t)num;
    } else if (qualifier == 'h') {
      num = (uint16_t)va_arg(args, int32_t);
      if (flags & SIGN)
        num = (int16_t)num;
    } else {
      num = va_arg(args, uint32_t);
      if (flags & SIGN)
        num = (int32_t)num;
    }
#ifdef RT_PRINTF_PRECISION
    str = print_number(str, end, num, base, field_width, precision, flags);
#else
    str = print_number(str, end, num, base, field_width, flags);
#endif
  }

  if (size > 0) {
    if (str < end)
      *str = '\0';
    else {
      end[-1] = '\0';
    }
  }

  /* the trailing null byte doesn't count towards the total
   * ++str;
   */
  int32_t i = str - buf;
  va_end(args);
  uart_puts(buf);
  memset(buf, '\0', printf_buf_length);
  return i;
}
#endif