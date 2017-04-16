#pragma once

#include <string.h>
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <stdexcept>
#include "generator2.hpp"

template <size_t spectrum> class spic {

private:
  static size_t const pixel_size;

  static uint32_t discretize(const double& value);

  static uint32_t* permutation_(uint32_t size, generator2* mapper);
  static uint32_t* inversate_(uint32_t* permutation, uint32_t size);

protected:
  static uint8_t* shuffle_(uint8_t* pixels, uint32_t size, generator2* mapper);
  static uint8_t* unshuffle_(uint8_t* pixels, uint32_t size, generator2* mapper);

  static uint8_t* substitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv);
  static uint8_t* unsubstitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv);

public:
  virtual ~spic() = default;
  virtual uint8_t* encrypt(uint8_t* pixels, uint32_t size) const = 0;
  virtual uint8_t* decrypt(uint8_t* pixels, uint32_t size) const = 0;
};

template<size_t spectrum>
size_t const spic<spectrum>::pixel_size = spectrum * sizeof(uint8_t);

template <size_t spectrum>
inline uint32_t spic<spectrum>::discretize(const double& value) {
  static double TEN_TO_FIFTEEN = std::pow(10.0, 15);
  return std::abs(std::floor(TEN_TO_FIFTEEN * value));
}

template <size_t spectrum>
inline uint32_t* spic<spectrum>::permutation_(uint32_t size, generator2* mapper) {
  uint32_t* perm = new uint32_t[size];
  bool* has = new bool[size];
  memset(has, false, size * sizeof(bool));

  uint32_t max = size, i = 0;
  dvec2 p = mapper->current();

  uint32_t aux_x = 0, aux_y = 0;
  bool found = false;

  while (i < size) {
    found = false;

    aux_x = discretize(p.x) % size;
    aux_y = discretize(p.y) % size;

    if (!has[aux_x]) {
      perm[i++] = aux_x, has[aux_x] = true;
      found = true;
    }
    if (!has[aux_y]) {
      perm[i++] = aux_y, has[aux_y] = true;
      found = true;
    }

    if (!found) {
      uint32_t j = max - 1;
      for (; j >= 0 && has[j]; j--);
      max = j;
      perm[i++] = max , has[max] = true;
    }

    p = mapper->next();
  }

  return perm;
}

template <size_t spectrum>
inline uint32_t* spic<spectrum>::inversate_(uint32_t* permutation, uint32_t size) {
  uint32_t* inverse = new uint32_t[size];
  for (uint32_t i = 0; i < size; i++) {
    inverse[permutation[i]] = i;
  }
  return inverse;
}

template <size_t spectrum>
inline uint8_t* spic<spectrum>::shuffle_(uint8_t* pixels, uint32_t size, generator2* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* shuffled = new uint8_t[size];
  uint32_t* permutation = permutation_(size / spectrum, mapper);

  for (uint32_t i = 0; i < size / spectrum; i++) {
    memcpy(&shuffled[permutation[i] * spectrum], &pixels[i * spectrum], pixel_size);
  }

  delete[] permutation;

  return shuffled;
}

template <size_t spectrum>
inline uint8_t* spic<spectrum>::unshuffle_(uint8_t* pixels, uint32_t size, generator2* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* unshuffled = new uint8_t[size];

  uint32_t* permutation = permutation_(size / spectrum, mapper);
  uint32_t* inverse = inversate_(permutation, size / spectrum);

  for (uint32_t i = 0; i < size / spectrum; i++) {
    memcpy(&unshuffled[inverse[i] * spectrum], &pixels[i * spectrum], pixel_size);
  }

  delete[] permutation, delete[] inverse;

  return unshuffled;
}

template <size_t spectrum>
inline uint8_t* spic<spectrum>::substitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* encrypted = new uint8_t[size];

  dvec2 p2 = mapper->current();
  uint32_t ks1 = discretize(p2.x);
  uint32_t ks2 = discretize(p2.y);

  uint32_t curr_pixel = 0;
  memcpy(&curr_pixel, &pixels[0], pixel_size);

  uint32_t encr_pixel = iv ^ curr_pixel ^ ks1 ^ ks2;
  memcpy(&encrypted[0], &encr_pixel, pixel_size);

  uint32_t prev_pixel = encr_pixel;

  uint32_t channels = 0;

  for (uint32_t i = spectrum; i < size; i += spectrum) {
    p2 = mapper->next();

    ks1 = discretize(p2.x);
    ks2 = discretize(p2.y);

    for (size_t c = 0; c < spectrum; c++) {
      channels += encrypted[i - spectrum + c];
    }

    memcpy(&curr_pixel, &pixels[i], pixel_size);
    encr_pixel = ((curr_pixel ^ ks1) + channels) ^ prev_pixel ^ ks2;
    prev_pixel = encr_pixel;

    memcpy(&encrypted[i], &encr_pixel, pixel_size);
  }

  return encrypted;
}

template <size_t spectrum>
inline uint8_t* spic<spectrum>::unsubstitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* decrypted = new uint8_t[size];

  dvec2 p2 = mapper->current();
  uint32_t ks1 = discretize(p2.x);
  uint32_t ks2 = discretize(p2.y);

  uint32_t curr_pixel = 0;
  memcpy(&curr_pixel, &pixels[0], pixel_size);

  uint32_t prev_pixel = curr_pixel;

  uint32_t decr_pixel = iv ^ curr_pixel ^ ks1 ^ ks2;
  memcpy(&decrypted[0], &decr_pixel, pixel_size);

  uint32_t channels = 0;

  for (uint32_t i = spectrum; i < size; i += spectrum) {
    p2 = mapper->next();

    ks1 = discretize(p2.x);
    ks2 = discretize(p2.y);

    for (size_t c = 0; c < spectrum; c++) {
      channels += pixels[i - spectrum + c];
    }

    memcpy(&curr_pixel, &pixels[i], pixel_size);
    decr_pixel = ((prev_pixel ^ curr_pixel ^ ks2) - channels) ^ ks1;
    prev_pixel = curr_pixel;

    memcpy(&decrypted[i], &decr_pixel, pixel_size);
  }

  return decrypted;
}
