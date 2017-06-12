#pragma once

#include <string.h>
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <stdexcept>

#include "image_cipher_base.hpp"
#include "generator2.hpp"
#include "spic_key.hpp"

template <size_t spectrum>
class spic_cipher: public image_cipher_base<spectrum> {

private:
  static uint32_t discretize_(const double& value);

  static uint32_t* permutation_(uint32_t size, generator2* mapper);
  static uint32_t* inversate_(uint32_t* permutation, uint32_t size);

protected:
  static uint8_t* shuffle_(uint8_t* pixels, uint32_t size, generator2* mapper);
  static uint8_t* unshuffle_(uint8_t* pixels, uint32_t size, generator2* mapper);

  static uint8_t* substitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv);
  static uint8_t* unsubstitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv);

  static uint8_t* do_encryption_(uint8_t* pixels, uint32_t size,
                                 generator2* mapper1, generator2* mapper2, uint32_t iv);
  static uint8_t* do_decryption_(uint8_t* pixels, uint32_t size,
                                 generator2* mapper1, generator2* mapper2, uint32_t iv);
};

template <size_t spectrum>
inline uint32_t spic_cipher<spectrum>::discretize_(const double& value) {
  static double TEN_TO_FIFTEEN = std::pow(10.0, 15);
  return std::abs(std::floor(TEN_TO_FIFTEEN * value));
}

template <size_t spectrum>
inline uint32_t* spic_cipher<spectrum>::permutation_(uint32_t size, generator2* mapper) {
  uint32_t* perm = new uint32_t[size];
  bool* has = new bool[size];
  memset(has, false, size * sizeof(bool));

  uint32_t max = size, i = 0;
  dvec2 p = mapper->current();

  uint32_t aux_x = 0, aux_y = 0;
  bool found = false;

  while (i < size) {
    found = false;

    aux_x = discretize_(p.x) % size;
    aux_y = discretize_(p.y) % size;

    if (!has[aux_x]) {
      perm[i++] = aux_x , has[aux_x] = true;
      found = true;
    }
    if (!has[aux_y]) {
      perm[i++] = aux_y , has[aux_y] = true;
      found = true;
    }

    if (!found) {
      uint32_t j = max - 1;
      for (; j > 0 && has[j]; j--);
      max = j;
      perm[i++] = max , has[max] = true;
    }

    p = mapper->next();
  }

  return perm;
}

template <size_t spectrum>
inline uint32_t* spic_cipher<spectrum>::inversate_(uint32_t* permutation, uint32_t size) {
  uint32_t* inverse = new uint32_t[size];
  for (uint32_t i = 0; i < size; i++) { inverse[permutation[i]] = i; }
  return inverse;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::shuffle_(uint8_t* pixels, uint32_t size, generator2* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* shuffled = new uint8_t[size];
  uint32_t* permutation = permutation_(size / spectrum, mapper);

  uint32_t s_idx = 0, p_idx = 0;
  for (uint32_t idx = 0; idx < size / spectrum; idx++) {
    s_idx = permutation[idx] * spectrum , p_idx = idx * spectrum;
    memcpy(&shuffled[s_idx], &pixels[p_idx], spic_cipher<spectrum>::pixel_size);
  }

  delete[] permutation;

  return shuffled;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::unshuffle_(uint8_t* pixels, uint32_t size, generator2* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* unshuffled = new uint8_t[size];

  uint32_t* permutation = permutation_(size / spectrum, mapper);
  uint32_t* inverse = inversate_(permutation, size / spectrum);

  uint32_t s_idx = 0, p_idx = 0;
  for (uint32_t idx = 0; idx < size / spectrum; idx++) {
    s_idx = inverse[idx] * spectrum , p_idx = idx * spectrum;
    memcpy(&unshuffled[s_idx], &pixels[p_idx], spic_cipher<spectrum>::pixel_size);
  }

  delete[] permutation , delete[] inverse;

  return unshuffled;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::substitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* encrypted = new uint8_t[size];

  // Declarations
  uint32_t channels = 0, ks1 = 0, ks2 = 0;
  uint8_t *ks1_pixel, *ks2_pixel;
  uint8_t* aux_pixel = reinterpret_cast<uint8_t*>(&iv);

  dvec2 p2 = mapper->current();

  for (uint32_t idx = 0; idx < size; idx += spectrum) {
    ks1 = discretize_(p2.x) , ks1_pixel = reinterpret_cast<uint8_t*>(&ks1);
    ks2 = discretize_(p2.y) , ks2_pixel = reinterpret_cast<uint8_t*>(&ks2);

    for (size_t c = 0; c < spectrum; c++) {
      uint8_t intermediate = (pixels[idx + c] ^ ks1_pixel[c]) + channels;
      encrypted[idx + c] = intermediate ^ aux_pixel[c] ^ ks2_pixel[c];
    }
    for (size_t c = 0; c < spectrum; c++) {
      channels += encrypted[idx + c];
    }

    aux_pixel = encrypted + idx , p2 = mapper->next();
  }

  return encrypted;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::unsubstitute_(uint8_t* pixels, uint32_t size, generator2* mapper, uint32_t iv) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* decrypted = new uint8_t[size];

  // Declarations
  uint32_t channels = 0, ks1 = 0, ks2 = 0;
  uint8_t *ks1_pixel, *ks2_pixel;
  uint8_t* aux_pixel = reinterpret_cast<uint8_t*>(&iv);

  dvec2 p2 = mapper->current();

  for (uint32_t idx = 0; idx < size; idx += spectrum) {
    ks1 = discretize_(p2.x) , ks1_pixel = reinterpret_cast<uint8_t*>(&ks1);
    ks2 = discretize_(p2.y) , ks2_pixel = reinterpret_cast<uint8_t*>(&ks2);

    for (size_t c = 0; c < spectrum; c++) {
      uint8_t intermediate = (pixels[idx + c] ^ ks2_pixel[c] ^ aux_pixel[c]) - channels;
      decrypted[idx + c] = intermediate ^ ks1_pixel[c];
    }
    for (size_t c = 0; c < spectrum; c++) {
      channels += pixels[idx + c];
    }

    aux_pixel = pixels + idx , p2 = mapper->next();
  }

  return decrypted;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::do_decryption_(uint8_t* pixels, uint32_t size,
                                               generator2* mapper1, generator2* mapper2, uint32_t iv) {
  uint8_t* unsubstituted = unsubstitute_(pixels, size, mapper2, iv);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, mapper1);

  delete[] unsubstituted;
  return unshuffled;
}

template <size_t spectrum>
inline uint8_t* spic_cipher<spectrum>::do_encryption_(uint8_t* pixels, uint32_t size,
                                               generator2* mapper1, generator2* mapper2, uint32_t iv) {
  uint8_t* shuffled = shuffle_(pixels, size, mapper1);
  uint8_t* substituted = substitute_(shuffled, size, mapper2, iv);

  delete[] shuffled;
  return substituted;
}
