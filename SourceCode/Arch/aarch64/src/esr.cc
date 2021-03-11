#include "aarch64/esr.hh"

extern const char* esr_class_str[];
const char* esr_get_class_string(u32 esr) { return esr_class_str[ESR_ELx_EC(esr)]; }
