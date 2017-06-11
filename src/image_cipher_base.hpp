#pragma once

#include <stdint.h>

template <size_t spectrum> class image_cipher_base {

protected:
  static size_t const pixel_size;

public:
  virtual ~image_cipher_base() = default;
  virtual uint8_t* encrypt(uint8_t* pixels, uint32_t size) const = 0;
  virtual uint8_t* decrypt(uint8_t* pixels, uint32_t size) const = 0;
};

template <size_t spectrum>
size_t const image_cipher_base<spectrum>::pixel_size = spectrum * sizeof(uint8_t);