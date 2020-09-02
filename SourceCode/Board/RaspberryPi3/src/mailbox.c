#include <stdbool.h>
#include <stdint.h>

#include "raspi3/mailbox.h"

void mailbox_call(uint32_t channel, void *data) {
  // wait until it's not full
  while (*MAILBOX0_STATUS & MAILBOX_FULL) {
  }
  // send message to mailbox
  *MAILBOX0_WRITE = ((channel & 0xF) | (((uint32_t)((unsigned long)data)) & ~0xf));
}

bool is_valid_mailbox_response(uint32_t code, uint32_t tag_code) {
  return code == MAILBOX_CODE_RESPONSE_SUCCESS && tag_code & MAILBOX_CODE_RESPONSE_SUCCESS;
}
