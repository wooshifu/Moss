
#if 0

#if 0
extern_C int printk(const char* fmt, ...) {
  va_list args;
  int r;

  va_start(args, fmt);
  r = vprintk_func(fmt, args);
  va_end(args);

  return r;
}

/*__printf(1, 0)*/ int vprintk_func(const char *fmt, va_list args)
{

  /* Use extra buffer to prevent a recursion deadlock in safe mode. */
  if (this_cpu_read(printk_context) & PRINTK_SAFE_CONTEXT_MASK)
    return vprintk_safe(fmt, args);

  /* No obstacles. */
  return vprintk_default(fmt, args);
}

int vprintk_default(const char *fmt, va_list args)
{
  return vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, 0, fmt, args);
}

asmlinkage int vprintk_emit(int facility, int level,
                            const char *dict, size_t dictlen,
                            const char *fmt, va_list args)
{
  int printed_len= vprintk_store(facility, level, dict, dictlen, fmt, args);
  return printed_len;
}

int vprintk_store(int facility, int level,
                  const char *dict, size_t dictlen,
                  const char *fmt, va_list args) {
  static char textbuf[LOG_LINE_MAX];
  char* text = textbuf;
  size_t text_len;
  enum log_flags lflags = 0;

  /*
   * The printf needs to come first; we need the syslog
   * prefix which might be passed-in as a parameter.
   */
  text_len = vscnprintf(text, sizeof(textbuf), fmt, args);
  return text_len;
}

int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
  size_t i;

  i = vsnprintf(buf, size, fmt, args);

  if (likely(i < size))
    return i;
  if (size != 0)
    return size - 1;
  return 0;
}
#endif

#if 0
/*
 * min()/max()/clamp() macros must accomplish three things:
 *
 * - avoid multiple evaluations of the arguments (so side-effects like
 *   "x++" happen only once) when non-constant.
 * - perform strict type-checking (to generate warnings instead of
 *   nasty runtime surprises). See the "unnecessary" pointer comparison
 *   in __typecheck().
 * - retain result as a constant expressions when called with only
 *   constant expressions (to avoid tripping VLA warnings in stack
 *   allocation usage).
 */
#define __typecheck(x, y) (!!(sizeof((__typeof__(x)*)1 == (__typeof__(y)*)1)))

/*
 * This returns a constant expression while determining if an argument is
 * a constant expression, most importantly without evaluating the argument.
 * Glory to Martin Uecker <Martin.Uecker@med.uni-goettingen.de>
 */
#define __is_constexpr(x) (sizeof(int) == sizeof(*(8 ? ((void*)((long)(x)*0l)) : (int*)8)))

#define __no_side_effects(x, y) (__is_constexpr(x) && __is_constexpr(y))

#define __safe_cmp(x, y) (__typecheck(x, y) && __no_side_effects(x, y))

#define __cmp(x, y, op) ((x)op(y) ? (x) : (y))

#define __cmp_once(x, y, unique_x, unique_y, op)                                                                       \
  ({                                                                                                                   \
    __typeof__(x) unique_x = (x);                                                                                      \
    __typeof__(y) unique_y = (y);                                                                                      \
    __cmp(unique_x, unique_y, op);                                                                                     \
  })

#define __careful_cmp(x, y, op)                                                                                        \
  __builtin_choose_expr(__safe_cmp(x, y), __cmp(x, y, op), __cmp_once(x, y, __UNIQUE_ID(__x), __UNIQUE_ID(__y), op))
#define __safe_cmp(x, y) (__typecheck(x, y) && __no_side_effects(x, y))

#define __cmp_once(x, y, unique_x, unique_y, op)                                                                       \
  ({                                                                                                                   \
    __typeof__(x) unique_x = (x);                                                                                      \
    __typeof__(y) unique_y = (y);                                                                                      \
    __cmp(unique_x, unique_y, op);                                                                                     \
  })

#define __careful_cmp(x, y, op)                                                                                        \
  __builtin_choose_expr(__safe_cmp(x, y), __cmp(x, y, op), __cmp_once(x, y, __UNIQUE_ID(__x), __UNIQUE_ID(__y), op))

//#define min(x, y)          __careful_cmp(x, y, <)
//#define max(x, y)       __careful_cmp(x, y, >)
#endif

#define min(x, y)          ((x) > (y) ? (y) : (x))
#define max(x, y)          ((x) < (y) ? (y) : (x))
#define clamp(val, lo, hi) min((__typeof__(val))max(val, lo), hi)

void* memcpy(void* dest, const void* src, size_t count) {
  char* tmp     = static_cast<char*>(dest);
  const char* s = static_cast<const char*>(src);

  while (count--)
    *tmp++ = *s++;
  return dest;
}

//static inline int __isdigit(int c) { return '0' <= c && c <= '9'; }
//#define isdigit(c) __isdigit(c)

[[gnu::noinline]] static int skip_atoi(const char** s) {
  int i = 0;

  do {
    i = i * 10 + *((*s)++) - '0';
  } while (isdigit(**s));

  return i;
}

#define SIGN    1  /* unsigned/signed, must be 1 */
#define LEFT    2  /* left justified */
#define PLUS    4  /* show plus */
#define SPACE   8  /* space if plus */
#define ZEROPAD 16 /* pad with zero, must be 16 == '0' - ' ' */
#define SMALL   32 /* use lowercase in hex (must be 32 == 0x20) */
#define SPECIAL 64 /* prefix hex with "0x", octal with "0" */

struct [[gnu::packed]] printf_spec {
  unsigned int type      : 8;  /* format_type enum */
  signed int field_width : 24; /* width of output field */
  unsigned int flags     : 8;  /* flags to number() */
  unsigned int base      : 8;  /* number base, 8, 10 or 16 only */
  signed int precision   : 16; /* # of digits/chars */
} /*__packed*/;
static_assert(sizeof(struct printf_spec) == 8);

#define FIELD_WIDTH_MAX ((1 << 23) - 1)
#define PRECISION_MAX   ((1 << 15) - 1)

enum format_type {
  FORMAT_TYPE_NONE, /* Just a string part */
  FORMAT_TYPE_WIDTH,
  FORMAT_TYPE_PRECISION,
  FORMAT_TYPE_CHAR,
  FORMAT_TYPE_STR,
  FORMAT_TYPE_PTR,
  FORMAT_TYPE_PERCENT_CHAR,
  FORMAT_TYPE_INVALID,
  FORMAT_TYPE_LONG_LONG,
  FORMAT_TYPE_ULONG,
  FORMAT_TYPE_LONG,
  FORMAT_TYPE_UBYTE,
  FORMAT_TYPE_BYTE,
  FORMAT_TYPE_USHORT,
  FORMAT_TYPE_SHORT,
  FORMAT_TYPE_UINT,
  FORMAT_TYPE_INT,
  FORMAT_TYPE_SIZE_T,
  FORMAT_TYPE_PTRDIFF
};

#define WARN_ONCE(...)
#define BUILD_BUG_ON(...)

constexpr int PAGE_SIZE = 4096;
#define MAX_ERRNO       4095
#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)


static void move_right(char *buf, char *end, unsigned len, unsigned spaces)
{
  size_t size;
  if (buf >= end)	/* nowhere to put anything */
    return;
  size = end - buf;
  if (size <= spaces) {
    memset(buf, ' ', size);
    return;
  }
  if (len) {
    if (len > size - spaces)
      len = size - spaces;
    memmove(buf + spaces, buf, len);
  }
  memset(buf, ' ', spaces);
}

static /*noinline_for_stack*/
char *widen_string(char *buf, int n, char *end, struct printf_spec spec)
{
  unsigned spaces;

  if (likely(n >= spec.field_width))
    return buf;
  /* we want to pad the sucker */
  spaces = spec.field_width - n;
  if (!(spec.flags & LEFT)) {
    move_right(buf - n, end, n, spaces);
    return buf + spaces;
  }
  while (spaces--) {
    if (buf < end)
      *buf = ' ';
    ++buf;
  }
  return buf;
}
static const char *check_pointer_msg(const void *ptr)
{
  if (!ptr)
    return "(null)";

  if ((unsigned long)ptr < PAGE_SIZE || IS_ERR_VALUE((unsigned long)ptr))
    return "(efault)";

  return nullptr;
}
/* Handle string from a well known address. */
static char *string_nocheck(char *buf, char *end, const char *s,
                            struct printf_spec spec)
{
  int len = 0;
  int lim = spec.precision;

  while (lim--) {
    char c = *s++;
    if (!c)
      break;
    if (buf < end)
      *buf = c;
    ++buf;
    ++len;
  }
  return widen_string(buf, len, end, spec);
}
/* Be careful: error messages must fit into the given buffer. */
static char *error_string(char *buf, char *end, const char *s,
                          struct printf_spec spec)
{
  /*
   * Hard limit to avoid a completely insane messages. It actually
   * works pretty well because most error messages are in
   * the many pointer format modifiers.
   */
  if (spec.precision == -1)
    spec.precision = 2 * sizeof(void *);

  return string_nocheck(buf, end, s, spec);
}
constexpr  int EFAULT = 14;
static int check_pointer(char **buf, char *end, const void *ptr,
                         struct printf_spec spec)
{
  const char *err_msg;

  err_msg = check_pointer_msg(ptr);
  if (err_msg) {
    *buf = error_string(*buf, end, err_msg, spec);
    return -EFAULT;
  }

  return 0;
}

static /*noinline_for_stack*/
char *string(char *buf, char *end, const char *s,
             struct printf_spec spec)
{
  if (check_pointer(&buf, end, s, spec))
    return buf;

  return string_nocheck(buf, end, s, spec);
}
const char hex_asc[] = "0123456789abcdef";
const char hex_asc_upper[] = "0123456789ABCDEF";


//# define __force	__attribute__((force))
#define __force

#define cpu_to_le16           __cpu_to_le16
using __le16 = u16;
using __u16 = u16;
#define ___constant_swab16(x) ((__u16)((((__u16)(x) & (__u16)0x00ffU) << 8) | (((__u16)(x) & (__u16)0xff00U) >> 8)))

static inline /*__attribute_const__*/ __u16 __fswab16(__u16 val)
{
#if defined(__arch_swab16)
return __arch_swab16(val);
#else
return ___constant_swab16(val);
#endif
}
//#define __fswab16(val) ___constant_swab16(val)

#ifdef __HAVE_BUILTIN_BSWAP16__
#define __swab16(x) (__u16) __builtin_bswap16((__u16)(x))
#else
#define __swab16(x) (__builtin_constant_p((__u16)(x)) ? ___constant_swab16(x) : __fswab16(x))
#endif
#define __cpu_to_le16(x) ((__force __le16)__swab16((x)))
static const u16 decpair[100] = {
#define _(x)             (__force u16) cpu_to_le16(((x % 10) | ((x / 10) << 8)) + 0x3030)
    _( 0), _( 1), _( 2), _( 3), _( 4), _( 5), _( 6), _( 7), _( 8), _( 9),
    _(10), _(11), _(12), _(13), _(14), _(15), _(16), _(17), _(18), _(19),
    _(20), _(21), _(22), _(23), _(24), _(25), _(26), _(27), _(28), _(29),
    _(30), _(31), _(32), _(33), _(34), _(35), _(36), _(37), _(38), _(39),
    _(40), _(41), _(42), _(43), _(44), _(45), _(46), _(47), _(48), _(49),
    _(50), _(51), _(52), _(53), _(54), _(55), _(56), _(57), _(58), _(59),
    _(60), _(61), _(62), _(63), _(64), _(65), _(66), _(67), _(68), _(69),
    _(70), _(71), _(72), _(73), _(74), _(75), _(76), _(77), _(78), _(79),
    _(80), _(81), _(82), _(83), _(84), _(85), _(86), _(87), _(88), _(89),
    _(90), _(91), _(92), _(93), _(94), _(95), _(96), _(97), _(98), _(99),
#undef _
};
/*
 * This will print a single '0' even if r == 0, since we would
 * immediately jump to out_r where two 0s would be written but only
 * one of them accounted for in buf. This is needed by ip4_string
 * below. All other callers pass a non-zero value of r.
*/
static /*noinline_for_stack*/
char *put_dec_trunc8(char *buf, unsigned r)
{
  unsigned q;

  /* 1 <= r < 10^8 */
  if (r < 100)
    goto out_r;

  /* 100 <= r < 10^8 */
  q = (r * (u64)0x28f5c29) >> 32;
  *((u16 *)buf) = decpair[r - 100*q];
  buf += 2;

  /* 1 <= q < 10^6 */
  if (q < 100)
    goto out_q;

  /*  100 <= q < 10^6 */
  r = (q * (u64)0x28f5c29) >> 32;
  *((u16 *)buf) = decpair[q - 100*r];
  buf += 2;

  /* 1 <= r < 10^4 */
  if (r < 100)
    goto out_r;

  /* 100 <= r < 10^4 */
  q = (r * 0x147b) >> 19;
  *((u16 *)buf) = decpair[r - 100*q];
  buf += 2;
  out_q:
  /* 1 <= q < 100 */
  r = q;
  out_r:
  /* 1 <= r < 100 */
  *((u16 *)buf) = decpair[r];
  buf += r < 10 ? 1 : 2;
  return buf;
}

#define BITS_PER_LONG      64
#define BITS_PER_LONG_LONG 64
#if BITS_PER_LONG == 64 && BITS_PER_LONG_LONG == 64
static /*noinline_for_stack*/
char *put_dec_full8(char *buf, unsigned r)
{
	unsigned q;

	/* 0 <= r < 10^8 */
	q = (r * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;

	/* 0 <= q < 10^6 */
	r = (q * (u64)0x28f5c29) >> 32;
	*((u16 *)buf) = decpair[q - 100*r];
	buf += 2;

	/* 0 <= r < 10^4 */
	q = (r * 0x147b) >> 19;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;

	/* 0 <= q < 100 */
	*((u16 *)buf) = decpair[q];
	buf += 2;
	return buf;
}

#if BITS_PER_LONG == 64
/**
 * do_div - returns 2 values: calculate remainder and update new dividend
 * @n: uint64_t dividend (will be updated)
 * @base: uint32_t divisor
 *
 * Summary:
 * ``uint32_t remainder = n % base;``
 * ``n = n / base;``
 *
 * Return: (uint32_t)remainder
 *
 * NOTE: macro parameter @n is evaluated multiple times,
 * beware of side effects!
 */
#define do_div(n, base)                                                                                                \
  ({                                                                                                                   \
    uint32_t __base = (base);                                                                                          \
    uint32_t __rem;                                                                                                    \
    __rem = ((uint64_t)(n)) % __base;                                                                                  \
    (n)   = ((uint64_t)(n)) / __base;                                                                                  \
    __rem;                                                                                                             \
  })
#endif

static /*noinline_for_stack*/
char *put_dec(char *buf, unsigned long long n)
{
	if (n >= 100*1000*1000)
		buf = put_dec_full8(buf, do_div(n, 100*1000*1000));
	/* 1 <= n <= 1.6e11 */
	if (n >= 100*1000*1000)
		buf = put_dec_full8(buf, do_div(n, 100*1000*1000));
	/* 1 <= n < 1e8 */
	return put_dec_trunc8(buf, n);
}

#elif BITS_PER_LONG == 32 && BITS_PER_LONG_LONG == 64

static void
put_dec_full4(char *buf, unsigned r)
{
	unsigned q;

	/* 0 <= r < 10^4 */
	q = (r * 0x147b) >> 19;
	*((u16 *)buf) = decpair[r - 100*q];
	buf += 2;
	/* 0 <= q < 100 */
	*((u16 *)buf) = decpair[q];
}

/*
 * Call put_dec_full4 on x % 10000, return x / 10000.
 * The approximation x/10000 == (x * 0x346DC5D7) >> 43
 * holds for all x < 1,128,869,999.  The largest value this
 * helper will ever be asked to convert is 1,125,520,955.
 * (second call in the put_dec code, assuming n is all-ones).
 */
static noinline_for_stack
unsigned put_dec_helper4(char *buf, unsigned x)
{
        uint32_t q = (x * (uint64_t)0x346DC5D7) >> 43;

        put_dec_full4(buf, x - q * 10000);
        return q;
}

/* Based on code by Douglas W. Jones found at
 * <http://www.cs.uiowa.edu/~jones/bcd/decimal.html#sixtyfour>
 * (with permission from the author).
 * Performs no 64-bit division and hence should be fast on 32-bit machines.
 */
static
char *put_dec(char *buf, unsigned long long n)
{
	uint32_t d3, d2, d1, q, h;

	if (n < 100*1000*1000)
		return put_dec_trunc8(buf, n);

	d1  = ((uint32_t)n >> 16); /* implicit "& 0xffff" */
	h   = (n >> 32);
	d2  = (h      ) & 0xffff;
	d3  = (h >> 16); /* implicit "& 0xffff" */

	/* n = 2^48 d3 + 2^32 d2 + 2^16 d1 + d0
	     = 281_4749_7671_0656 d3 + 42_9496_7296 d2 + 6_5536 d1 + d0 */
	q   = 656 * d3 + 7296 * d2 + 5536 * d1 + ((uint32_t)n & 0xffff);
	q = put_dec_helper4(buf, q);

	q += 7671 * d3 + 9496 * d2 + 6 * d1;
	q = put_dec_helper4(buf+4, q);

	q += 4749 * d3 + 42 * d2;
	q = put_dec_helper4(buf+8, q);

	q += 281 * d3;
	buf += 12;
	if (q)
		buf = put_dec_trunc8(buf, q);
	else while (buf[-1] == '0')
		--buf;

	return buf;
}

#endif


static /*noinline_for_stack*/
char *number(char *buf, char *end, unsigned long long num,
             struct printf_spec spec)
{
  /* put_dec requires 2-byte alignment of the buffer. */
  char tmp[3 * sizeof(num)] __attribute__((aligned(2)));
  char sign;
  char locase;
  int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10);
  int i;
  bool is_zero = num == 0LL;
  int field_width = spec.field_width;
  int precision = spec.precision;

  /* locase = 0 or 0x20. ORing digits or letters with 'locase'
   * produces same digits or (maybe lowercased) letters */
  locase = (spec.flags & SMALL);
  if (spec.flags & LEFT)
    spec.flags &= ~ZEROPAD;
  sign = 0;
  if (spec.flags & SIGN) {
    if ((signed long long)num < 0) {
      sign = '-';
      num = -(signed long long)num;
      field_width--;
    } else if (spec.flags & PLUS) {
      sign = '+';
      field_width--;
    } else if (spec.flags & SPACE) {
      sign = ' ';
      field_width--;
    }
  }
  if (need_pfx) {
    if (spec.base == 16)
      field_width -= 2;
    else if (!is_zero)
      field_width--;
  }

  /* generate full string in tmp[], in reverse order */
  i = 0;
  if (num < spec.base)
    tmp[i++] = hex_asc_upper[num] | locase;
  else if (spec.base != 10) { /* 8 or 16 */
    int mask = spec.base - 1;
    int shift = 3;

    if (spec.base == 16)
      shift = 4;
    do {
      tmp[i++] = (hex_asc_upper[((unsigned char)num) & mask] | locase);
      num >>= shift;
    } while (num);
  } else { /* base 10 */
    i = put_dec(tmp, num) - tmp;
  }

  /* printing 100 using %2d gives "100", not "00" */
  if (i > precision)
    precision = i;
  /* leading space padding */
  field_width -= precision;
  if (!(spec.flags & (ZEROPAD | LEFT))) {
    while (--field_width >= 0) {
      if (buf < end)
        *buf = ' ';
      ++buf;
    }
  }
  /* sign */
  if (sign) {
    if (buf < end)
      *buf = sign;
    ++buf;
  }
  /* "0x" / "0" prefix */
  if (need_pfx) {
    if (spec.base == 16 || !is_zero) {
      if (buf < end)
        *buf = '0';
      ++buf;
    }
    if (spec.base == 16) {
      if (buf < end)
        *buf = ('X' | locase);
      ++buf;
    }
  }
  /* zero or space padding */
  if (!(spec.flags & LEFT)) {
    char c = ' ' + (spec.flags & ZEROPAD);

    while (--field_width >= 0) {
      if (buf < end)
        *buf = c;
      ++buf;
    }
  }
  /* hmm even more zero padding? */
  while (i <= --precision) {
    if (buf < end)
      *buf = '0';
    ++buf;
  }
  /* actual digits of result */
  while (--i >= 0) {
    if (buf < end)
      *buf = tmp[i];
    ++buf;
  }
  /* trailing space padding */
  while (--field_width >= 0) {
    if (buf < end)
      *buf = ' ';
    ++buf;
  }

  return buf;
}

static /*noinline_for_stack*/
char *special_hex_number(char *buf, char *end, unsigned long long num, int size)
{
  struct printf_spec spec;

  spec.type = FORMAT_TYPE_PTR;
  spec.field_width = 2 + 2 * size;	/* 0x + hex */
  spec.flags = SPECIAL | SMALL | ZEROPAD;
  spec.base = 16;
  spec.precision = -1;

  return number(buf, end, num, spec);
}
static char *pointer_string(char *buf, char *end,
                            const void *ptr,
                            struct printf_spec spec)
{
  spec.base = 16;
  spec.flags |= SMALL;
  if (spec.field_width == -1) {
    spec.field_width = 2 * sizeof(ptr);
    spec.flags |= ZEROPAD;
  }

  return number(buf, end, (unsigned long int)ptr, spec);
}




/*
 * Helper function to decode printf style format.
 * Each call decode a token from the format and return the
 * number of characters read (or likely the delta where it wants
 * to go on the next call).
 * The decoded token is returned through the parameters
 *
 * 'h', 'l', or 'L' for integer fields
 * 'z' support added 23/7/1999 S.H.
 * 'z' changed to 'Z' --davidm 1/25/99
 * 'Z' changed to 'z' --adobriyan 2017-01-25
 * 't' added for ptrdiff_t
 *
 * @fmt: the format string
 * @type of the token returned
 * @flags: various flags such as +, -, # tokens..
 * @field_width: overwritten width
 * @base: base of the number (octal, hex, ...)
 * @precision: precision of a number
 * @qualifier: qualifier of a number (long, size_t, ...)
 */
[[gnu::noinline]] static int format_decode(const char* fmt, struct printf_spec* spec) {
  const char* start = fmt;
  char qualifier;

  /* we finished early by reading the field width */
  if (spec->type == FORMAT_TYPE_WIDTH) {
    if (spec->field_width < 0) {
      spec->field_width = -spec->field_width;
      spec->flags |= LEFT;
    }
    spec->type = FORMAT_TYPE_NONE;
    goto precision;
  }

  /* we finished early by reading the precision */
  if (spec->type == FORMAT_TYPE_PRECISION) {
    if (spec->precision < 0)
      spec->precision = 0;

    spec->type = FORMAT_TYPE_NONE;
    goto qualifier;
  }

  /* By default */
  spec->type = FORMAT_TYPE_NONE;

  for (; *fmt; ++fmt) {
    if (*fmt == '%')
      break;
  }

  /* Return the current non-format string */
  if (fmt != start || !*fmt)
    return fmt - start;

  /* Process flags */
  spec->flags = 0;

  while (true) { /* this also skips first '%' */
    bool found = true;

    ++fmt;

    switch (*fmt) {
    case '-':
      spec->flags |= LEFT;
      break;
    case '+':
      spec->flags |= PLUS;
      break;
    case ' ':
      spec->flags |= SPACE;
      break;
    case '#':
      spec->flags |= SPECIAL;
      break;
    case '0':
      spec->flags |= ZEROPAD;
      break;
    default:
      found = false;
    }

    if (!found)
      break;
  }

  /* get field width */
  spec->field_width = -1;

  if (isdigit(*fmt))
    spec->field_width = skip_atoi(&fmt);
  else if (*fmt == '*') {
    /* it's the next argument */
    spec->type = FORMAT_TYPE_WIDTH;
    return ++fmt - start;
  }

precision:
  /* get the precision */
  spec->precision = -1;
  if (*fmt == '.') {
    ++fmt;
    if (isdigit(*fmt)) {
      spec->precision = skip_atoi(&fmt);
      if (spec->precision < 0)
        spec->precision = 0;
    } else if (*fmt == '*') {
      /* it's the next argument */
      spec->type = FORMAT_TYPE_PRECISION;
      return ++fmt - start;
    }
  }

qualifier:
  /* get the conversion qualifier */
  qualifier = 0;
  if (*fmt == 'h' || _tolower(*fmt) == 'l' || *fmt == 'z' || *fmt == 't') {
    qualifier = *fmt++;
    if (unlikely(qualifier == *fmt)) {
      if (qualifier == 'l') {
        qualifier = 'L';
        ++fmt;
      } else if (qualifier == 'h') {
        qualifier = 'H';
        ++fmt;
      }
    }
  }

  /* default base */
  spec->base = 10;
  switch (*fmt) {
  case 'c':
    spec->type = FORMAT_TYPE_CHAR;
    return ++fmt - start;

  case 's':
    spec->type = FORMAT_TYPE_STR;
    return ++fmt - start;

  case 'p':
    spec->type = FORMAT_TYPE_PTR;
    return ++fmt - start;

  case '%':
    spec->type = FORMAT_TYPE_PERCENT_CHAR;
    return ++fmt - start;

    /* integer number formats - set up the flags and "break" */
  case 'o':
    spec->base = 8;
    break;

  case 'x':
    spec->flags |= SMALL;
    /* fall through */
        __attribute__((fallthrough));

  case 'X':
    spec->base = 16;
    break;

  case 'd':
  case 'i':
    spec->flags |= SIGN;
  case 'u':
    break;

  case 'n':
    /*
     * Since %n poses a greater security risk than
     * utility, treat it as any other invalid or
     * unsupported format specifier.
     */
    /* fall through */

  default:
//    WARN_ONCE(1, "Please remove unsupported %%%c in format string\n", *fmt);
    spec->type = FORMAT_TYPE_INVALID;
    return fmt - start;
  }

  if (qualifier == 'L')
    spec->type = FORMAT_TYPE_LONG_LONG;
  else if (qualifier == 'l') {
    BUILD_BUG_ON(FORMAT_TYPE_ULONG + SIGN != FORMAT_TYPE_LONG);
    spec->type = FORMAT_TYPE_ULONG + (spec->flags & SIGN);
  } else if (qualifier == 'z') {
    spec->type = FORMAT_TYPE_SIZE_T;
  } else if (qualifier == 't') {
    spec->type = FORMAT_TYPE_PTRDIFF;
  } else if (qualifier == 'H') {
    BUILD_BUG_ON(FORMAT_TYPE_UBYTE + SIGN != FORMAT_TYPE_BYTE);
    spec->type = FORMAT_TYPE_UBYTE + (spec->flags & SIGN);
  } else if (qualifier == 'h') {
    BUILD_BUG_ON(FORMAT_TYPE_USHORT + SIGN != FORMAT_TYPE_SHORT);
    spec->type = FORMAT_TYPE_USHORT + (spec->flags & SIGN);
  } else {
    BUILD_BUG_ON(FORMAT_TYPE_UINT + SIGN != FORMAT_TYPE_INT);
    spec->type = FORMAT_TYPE_UINT + (spec->flags & SIGN);
  }

  return ++fmt - start;
}

static void set_field_width(struct printf_spec* spec, int width) {
  spec->field_width = width;
//  if (WARN_ONCE(spec->field_width != width, "field width %d too large", width)) {
    if (spec->field_width != width) {
    spec->field_width = clamp(width, -FIELD_WIDTH_MAX, FIELD_WIDTH_MAX);
  }
}

static void set_precision(struct printf_spec* spec, int prec) {
  spec->precision = prec;
//  if (WARN_ONCE(spec->precision != prec, "precision %d too large", prec)) {
    if (spec->precision != prec) {
    spec->precision = clamp(prec, 0, PRECISION_MAX);
  }
}
static inline bool /*__must_check*/ IS_ERR_OR_NULL(__force const void *ptr)
{
  return unlikely(!ptr) || IS_ERR_VALUE((unsigned long)ptr);
}
#if 0
static char *ptr_to_id(char *buf, char *end, const void *ptr,
                       struct printf_spec spec)
{
  const char *str = sizeof(ptr) == 8 ? "(____ptrval____)" : "(ptrval)";
  unsigned long hashval;
  int ret;

  /*
   * Print the real pointer value for NULL and error pointers,
   * as they are not actual addresses.
   */
  if (IS_ERR_OR_NULL(ptr))
    return pointer_string(buf, end, ptr, spec);

  /* When debugging early boot use non-cryptographically secure hash. */
//  if (unlikely(debug_boot_weak_hash)) {
//    hashval = hash_long((unsigned long)ptr, 32);
//    return pointer_string(buf, end, (const void *)hashval, spec);
//  }

  ret = __ptr_to_hashval(ptr, &hashval);
  if (ret) {
    spec.field_width = 2 * sizeof(ptr);
    /* string length must be less than default_width */
    return error_string(buf, end, str, spec);
  }

  return pointer_string(buf, end, (const void *)hashval, spec);
}
#endif
static /*noinline_for_stack*/
char *pointer(const char *fmt, char *buf, char *end, void *ptr,
              struct printf_spec spec)
{
//  switch (*fmt) {
#if 0
  case 'S':
  case 's':
    ptr = dereference_symbol_descriptor(ptr);
    /* fall through */
  case 'B':
    return symbol_string(buf, end, ptr, spec, fmt);
  case 'R':
  case 'r':
    return resource_string(buf, end, ptr, spec, fmt);
  case 'h':
    return hex_string(buf, end, ptr, spec, fmt);
  case 'b':
    switch (fmt[1]) {
    case 'l':
      return bitmap_list_string(buf, end, ptr, spec, fmt);
    default:
      return bitmap_string(buf, end, ptr, spec, fmt);
    }
  case 'M':			/* Colon separated: 00:01:02:03:04:05 */
  case 'm':			/* Contiguous: 000102030405 */
    /* [mM]F (FDDI) */
    /* [mM]R (Reverse order; Bluetooth) */
    return mac_address_string(buf, end, ptr, spec, fmt);
  case 'I':			/* Formatted IP supported
					 * 4:	1.2.3.4
					 * 6:	0001:0203:...:0708
					 * 6c:	1::708 or 1::1.2.3.4
					 */
  case 'i':			/* Contiguous:
					 * 4:	001.002.003.004
					 * 6:   000102...0f
					 */
    return ip_addr_string(buf, end, ptr, spec, fmt);
  case 'E':
    return escaped_string(buf, end, ptr, spec, fmt);
  case 'U':
    return uuid_string(buf, end, ptr, spec, fmt);
  case 'V':
    return va_format(buf, end, ptr, spec, fmt);
  case 'K':
    return restricted_pointer(buf, end, ptr, spec);
  case 'N':
    return netdev_bits(buf, end, ptr, spec, fmt);
  case 'a':
    return address_val(buf, end, ptr, spec, fmt);
  case 'd':
    return dentry_name(buf, end, ptr, spec, fmt);
  case 't':
    return time_and_date(buf, end, ptr, spec, fmt);
  case 'C':
    return clock(buf, end, ptr, spec, fmt);
  case 'D':
    return file_dentry_name(buf, end, ptr, spec, fmt);
#ifdef CONFIG_BLOCK
    case 'g':
		return bdev_name(buf, end, ptr, spec, fmt);
#endif

  case 'G':
    return flags_string(buf, end, ptr, spec, fmt);
  case 'O':
    return device_node_string(buf, end, ptr, spec, fmt + 1);
  case 'f':
    return fwnode_string(buf, end, ptr, spec, fmt + 1);
#endif
//  case 'x':
    return pointer_string(buf, end, ptr, spec);
#if 0
  case 'e':
    /* %pe with a non-ERR_PTR gets treated as plain %p */
    if (!IS_ERR(ptr))
      break;
    return err_ptr(buf, end, ptr, spec);
  case 'u':
  case 'k':
    switch (fmt[1]) {
    case 's':
      return string(buf, end, ptr, spec);
    default:
      return error_string(buf, end, "(einval)", spec);
    }
  }
  /* default is to _not_ leak addresses, hash before printing */
  return ptr_to_id(buf, end, ptr, spec);
#endif
}
/**
 * vsnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * This function generally follows C99 vsnprintf, but has some
 * extensions and a few limitations:
 *
 *  - ``%n`` is unsupported
 *  - ``%p*`` is handled by pointer()
 *
 * See pointer() or Documentation/core-api/printk-formats.rst for more
 * extensive description.
 *
 * **Please update the documentation in both places when making changes**
 *
 * The return value is the number of characters which would
 * be generated for the given input, excluding the trailing
 * '\0', as per ISO C99. If you want to have the exact
 * number of characters written into @buf as return value
 * (not including the trailing '\0'), use vscnprintf(). If the
 * return is greater than or equal to @size, the resulting
 * string is truncated.
 *
 * If you're not already dealing with a va_list consider using snprintf().
 */
int vsnprintf(char* buf, size_t size, const char* fmt, va_list args) {
  unsigned long long num;
  struct printf_spec spec {};

  /* Reject out-of-range values early.  Large positive sizes are
     used for unknown buffer sizes. */
  //  if (WARN_ON_ONCE(size > INT_MAX))
  //    return 0;

  char* str = buf;
  char* end = buf + size;

  /* Make sure end is always >= buf */
  if (end < buf) {
    end  = static_cast<char*>((void*)-1);
    size = end - buf;
  }

  while (*fmt) {
    const char* old_fmt = fmt;
    int read            = format_decode(fmt, &spec);

    fmt += read;

    switch (spec.type) {
    case FORMAT_TYPE_NONE: {
      int copy = read;
      if (str < end) {
        if (copy > end - str)
          copy = end - str;
        memcpy(str, old_fmt, copy);
      }
      str += read;
      break;
    }

    case FORMAT_TYPE_WIDTH:
      set_field_width(&spec, va_arg(args, int));
      break;

    case FORMAT_TYPE_PRECISION:
      set_precision(&spec, va_arg(args, int));
      break;

    case FORMAT_TYPE_CHAR: {
      char c;

      if (!(spec.flags & LEFT)) {
        while (--spec.field_width > 0) {
          if (str < end)
            *str = ' ';
          ++str;
        }
      }
      c = (unsigned char)va_arg(args, int);
      if (str < end)
        *str = c;
      ++str;
      while (--spec.field_width > 0) {
        if (str < end)
          *str = ' ';
        ++str;
      }
      break;
    }

    case FORMAT_TYPE_STR:
      str = string(str, end, va_arg(args, char*), spec);
      break;

    case FORMAT_TYPE_PTR:
      str = pointer(fmt, str, end, va_arg(args, void*), spec);
      while (isalnum(*fmt))
        fmt++;
      break;

    case FORMAT_TYPE_PERCENT_CHAR:
      if (str < end)
        *str = '%';
      ++str;
      break;

    case FORMAT_TYPE_INVALID:
      /*
       * Presumably the arguments passed gcc's type
       * checking, but there is no safe or sane way
       * for us to continue parsing the format and
       * fetching from the va_list; the remaining
       * specifiers and arguments would be out of
       * sync.
       */
      goto out;

    default:
      switch (spec.type) {
      case FORMAT_TYPE_LONG_LONG:
        num = va_arg(args, long long);
        break;
      case FORMAT_TYPE_ULONG:
        num = va_arg(args, unsigned long);
        break;
      case FORMAT_TYPE_LONG:
        num = va_arg(args, long);
        break;
      case FORMAT_TYPE_SIZE_T:
        if (spec.flags & SIGN)
          num = va_arg(args, ssize_t);
        else
          num = va_arg(args, size_t);
        break;
      case FORMAT_TYPE_PTRDIFF:
        num = va_arg(args, ptrdiff_t);
        break;
      case FORMAT_TYPE_UBYTE:
        num = (unsigned char)va_arg(args, int);
        break;
      case FORMAT_TYPE_BYTE:
        num = (signed char)va_arg(args, int);
        break;
      case FORMAT_TYPE_USHORT:
        num = (unsigned short)va_arg(args, int);
        break;
      case FORMAT_TYPE_SHORT:
        num = (short)va_arg(args, int);
        break;
      case FORMAT_TYPE_INT:
        num = (int)va_arg(args, int);
        break;
      default:
        num = va_arg(args, unsigned int);
      }

      str = number(str, end, num, spec);
    }
  }

out:
  if (size > 0) {
    if (str < end)
      *str = '\0';
    else
      end[-1] = '\0';
  }

  /* the trailing null byte doesn't count towards the total */
  return str - buf;
}
#endif