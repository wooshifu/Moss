#include "hal/print.hh"
#include "kernel/logo.hh"

static const char *__moss_logo = "      ___           ___           ___           ___     \n"
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
  while (*__moss_logo) {
    put_char(*__moss_logo++);
  }
}
