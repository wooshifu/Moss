#include "hal/init.hh"      // for init_board, init_cpu
#include "hal/stdio.hh"     // for getchar
#include "libcxx/macro.hh"  // for extern_C
#include "libcxx/printf.hh" // for printf
#include "libcxx/stdarg.hh"

int32_t sprintf(char* buf, const char* format, ...);
void uart_puts(const char* s);

void swap(char* a, char* b) {
  char temp = *a;
  *a        = *b;
  *b        = temp;
}
/* A utility function to reverse a string  */
void reverse(char str[], int length) {
  int start = 0;
  int end   = length - 1;
  while (start < end) {
    swap(str + start, str + end);
    start++;
    end--;
  }
}
char* itoa(u64 num, char* str, int base) {
  int i           = 0;
  bool isNegative = false;

  /* Handle 0 explicitely, otherwise empty string is printed for 0 */
  if (num == 0) {
    str[i++] = '0';
    str[i]   = '\0';
    return str;
  }

  // In standard itoa(), negative numbers are handled only with
  // base 10. Otherwise numbers are considered unsigned.
  if (num < 0 && base == 10) {
    isNegative = true;
    num        = -num;
  }

  // Process individual digits
  while (num != 0) {
    int rem  = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num      = num / base;
  }

  // If number is negative, append '-'
  if (isNegative)
    str[i++] = '-';

  str[i] = '\0'; // Append string terminator

  // Reverse the string
  reverse(str, i);

  return str;
}
u64 test(int count, ...) {
  va_list va;
  va_start(va, count);
  u64 sum = 0;
  for (int i = 0; i < count; ++i) {
    u64 item = va_arg(va, u64);
    sum += item;
  }
  va_end(va);
  return sum;
}
int32_t my_printf(const char* fmt, ...);
[[noreturn]] extern_C void kernel_main() {
  init_board();
  uart_puts("kernel main\n");

  auto sum = test(5, 0x1234'0000'0000'0005LL, 0x1234'0000'0000'0006LL, 0x1234'0000'0000'0007LL, 0x1234'0000'0000'0008LL,
                  0x1234'0000'0000'0009LL);
  char ss[50]{};
  itoa(sum, ss, 10);
  uart_puts("sum is: ");
  uart_puts(ss);
  uart_puts("\n");
  if (sum == 15) {
    uart_puts("sum 15\n");
  } else {
    uart_puts("sum error\n");
  }

  const char* h = "hello";
  printf("test %s\n", "aaa");
  printf("test %d\n", 123);
  printf("test %p\n", h);

  uart_puts(">>>>>>>>> entered kernel_main <<<<<<<<<\n");

  printf("hello world %s\n", "xxx");
  printf("hello world %d\n", 1234);

  //  print_moss_logo();

  uart_puts((char*)"init cpu\n");
  init_cpu();

  //  uart_puts((char*)"current el: %llu", get_current_el());
  double x = 5.0;
  double y = 3.0;
  double z = x / y;
  //  log_d("5/3=%f", z);

  //   unmapped virtual memory invalid access test
  //   u64 v = *(u64*)0x16;
  uart_puts(">>>>>>>>> exited kernel_main <<<<<<<<<\n");
  uart_puts("this line of code should never be executed\n");

  uart_puts("> \n");
  while (true) {
    auto ch = getchar();
    if (ch == '\r' or ch == '\n') {
      uart_puts("\n> ");
      continue;
    }
    uart_puts("got %c");
  }
}
