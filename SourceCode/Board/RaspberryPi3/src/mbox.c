#include "mbox.h"
#include "log.h"

/*
mailbox_message_t mailbox_call(mailbox_message_t message) {
  while (*MAILBOX0_STATUS & MAILBOX_FULL) {
    // nop
  }
  mailbox_send(message);
  mailbox_message_t data = mailbox_read(message.channel);
  return data;
}

mailbox_message_t mailbox_read(uint8_t channel) {
  mailbox_status_t status;
  mailbox_message_t result;
  do {
    do {
      status.empty = *MAILBOX0_STATUS & MAILBOX_EMPTY;
      status.full = *MAILBOX0_STATUS & MAILBOX_FULL;
    } while (status.empty);
    result.channel = *MAILBOX0_READ & 0xF;
    result.data = *MAILBOX0_READ & ~0xF;
  } while (result.channel != channel);
  return result;
}
*/

void mailbox_send(mailbox_message_t* message) {
  //  mailbox_status_t status;
  //  do {
  //    status.empty = *MAILBOX0_STATUS & MAILBOX_EMPTY;
  //    status.full = *MAILBOX0_STATUS & MAILBOX_FULL;
  //  } while (status.full);

  // wait until it's not full
  while (*MAILBOX0_STATUS & MAILBOX_FULL) {
  }
  // send message to mailbox
  *MAILBOX0_WRITE = (message->channel | message->data);
  // now wait for the response
#if 0
  while (1) {
    /* is there a response? */
    do {
      //      asm volatile("nop");
      // wait until it's not empty
    } while (*MAILBOX0_STATUS & MAILBOX_EMPTY);
    /* is it a response to our message? */ // todo: check whether it's a response
    log_d("read: %d, message:%d, %p, data:%d", *MAILBOX0_READ,(unsigned long )message,message,message->data);
    if ((unsigned long )message == *MAILBOX0_READ) {
      /* is it a valid successful response? */
      //      return mailbox[1] == MAILBOX0_RESPONSE;
      //      MAILBOX_CODE_TAG_IS_RESPONSE(*((*(uint32_t*)(&message.data))+5));
      // todo: check message.data.property.code == MAILBOX_CODE_TAG_RESPONSE
      return;
    }
  }
#endif
}
