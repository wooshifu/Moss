#include "hal/stdio.hh"
#include "kernel/logo.hh"

constexpr const char* moss_logo = "      ___           ___           ___           ___     \n"
                                  "     /__/\\         /  /\\         /  /\\         /  /\\    \n"
                                  "    |  |::\\       /  /::\\       /  /:/_       /  /:/_   \n"
                                  "    |  |:|:\\     /  /:/\\:\\     /  /:/ /\\     /  /:/ /\\  \n"
                                  "  __|__|:|\\:\\   /  /:/  \\:\\   /  /:/ /::\\   /  /:/ /::\\ \n"
                                  " /__/::::| \\:\\ /__/:/ \\__\\:\\ /__/:/ /:/\\:\\ /__/:/ /:/\\:\\\n"
                                  " \\  \\:\\~~\\__\\/ \\  \\:\\ /  /:/ \\  \\:\\/:/~/:/ \\  \\:\\/:/~/:/\n"
                                  "  \\  \\:\\        \\  \\:\\  /:/   \\  \\::/ /:/   \\  \\::/ /:/ \n"
                                  "   \\  \\:\\        \\  \\:\\/:/     \\__\\/ /:/     \\__\\/ /:/  \n"
                                  "    \\  \\:\\        \\  \\::/        /__/:/        /__/:/   \n"
                                  "     \\__\\/         \\__\\/         \\__\\/         \\__\\/    \n"
                                  "                                                        \n";

void print_moss_logo() {
  const char* logo = moss_logo;
  while (*logo != '\0') {
    putchar(*logo++);
  }
}
