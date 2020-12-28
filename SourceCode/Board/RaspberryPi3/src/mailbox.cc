#include "libcxx/types.hh"

#include "rpi3/mailbox.hh"

void mailbox_call(u32 channel, void* data) {
  // wait until it's not full
  while (*MAILBOX0_STATUS & MAILBOX_FULL) {
  }
  // send message to mailbox
  *MAILBOX0_WRITE = ((channel & 0xF) | (((u32)((unsigned long)data)) & ~0xf));
}

bool is_valid_mailbox_response(u32 code, u32 tag_code) {
  return code == MAILBOX_CODE_RESPONSE_SUCCESS and tag_code & MAILBOX_CODE_RESPONSE_SUCCESS;
}
