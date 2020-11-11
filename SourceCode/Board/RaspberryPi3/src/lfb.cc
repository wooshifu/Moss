#if 0
#include "homer.hpp"
#include "log.hpp"
#include "mailbox.hpp"

unsigned int width, height, pitch, isrgb; /* dimensions and channel order */
unsigned char *lfb;                       /* raw frame buffer address */

/**
 * Set screen resolution to 1024x768
 */
void init_lfb() {
  mailbox[0] = 35 * 4;
  mailbox[1] = MAILBOX_REQUEST;

  mailbox[2] = 0x48003; // set phy wh
  mailbox[3] = 8;
  mailbox[4] = 8;
  mailbox[5] = 1024; // FrameBufferInfo.width
  mailbox[6] = 768;  // FrameBufferInfo.height

  mailbox[7] = 0x48004; // set virt wh
  mailbox[8] = 8;
  mailbox[9] = 8;
  mailbox[10] = 1024; // FrameBufferInfo.virtual_width
  mailbox[11] = 768;  // FrameBufferInfo.virtual_height

  mailbox[12] = 0x48009; // set virt offset
  mailbox[13] = 8;
  mailbox[14] = 8;
  mailbox[15] = 0; // FrameBufferInfo.x_offset
  mailbox[16] = 0; // FrameBufferInfo.y.offset

  mailbox[17] = 0x48005; // set depth
  mailbox[18] = 4;
  mailbox[19] = 4;
  mailbox[20] = 32; // FrameBufferInfo.depth

  mailbox[21] = 0x48006; // set pixel order
  mailbox[22] = 4;
  mailbox[23] = 4;
  mailbox[24] = 1; // RGB, not BGR preferably

  mailbox[25] = 0x40001; // get framebuffer, gets alignment on request
  mailbox[26] = 8;
  mailbox[27] = 8;
  mailbox[28] = 4096; // FrameBufferInfo.pointer
  mailbox[29] = 0;    // FrameBufferInfo.size

  mailbox[30] = 0x40008; // get pitch
  mailbox[31] = 4;
  mailbox[32] = 4;
  mailbox[33] = 0; // FrameBufferInfo.pitch

  mailbox[34] = MAILBOX_TAG_LAST;

  // this might not return exactly what we asked for, could be
  // the closest supported resolution instead
  if (mailbox_call(MAILBOX_CHANNEL_PROPERTY_ARM_TO_VIDEO_CORE) && mailbox[20] == 32 && mailbox[28] != 0) {
    mailbox[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
    width = mailbox[5];        // get actual physical width
    height = mailbox[6];       // get actual physical height
    pitch = mailbox[33];       // get number of bytes per line
    isrgb = mailbox[24];       // get the actual channel order
    lfb = (void *)((unsigned long)mailbox[28]);
  } else {
    log_e("Unable to set screen resolution to 1024x768x32");
  }
}

/**
 * Show a picture
 */
void lfb_showpicture() {
  unsigned char *ptr = lfb;
  char *data = homer_data;
  char pixel[4];

  ptr += (height - homer_height) / 2 * pitch + (width - homer_width) * 2;
  for (unsigned int y = 0; y < homer_height; y++) {
    for (unsigned int x = 0; x < homer_width; x++) {
      HEADER_PIXEL(data, pixel);
      // the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
      // directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
      *((unsigned int *)ptr) =
          isrgb ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0] << 16 | pixel[1] << 8 | pixel[2]);
      ptr += 4;
    }
    ptr += pitch - homer_width * 4;
  }
}

#endif