#include "libio/io.hh"

#include "libstd/error.hh" // for ERR_NOT_SUPPORTED

ssize_t io_write(io_handle_t* io, const char* buf, size_t len) {
  if (io->hooks->write == nullptr) {
    return ERR_NOT_SUPPORTED;
  }

  return io->hooks->write(io, buf, len);
}

ssize_t io_read(io_handle_t* io, char* buf, size_t len) {
  if (io->hooks->read == nullptr) {
    return ERR_NOT_SUPPORTED;
  }

  return io->hooks->read(io, buf, len);
}
