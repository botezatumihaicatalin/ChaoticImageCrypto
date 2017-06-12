#pragma once

#include <stdint.h>
#include <stdexcept>

#include "image_cipher_base.hpp"
#include "generator3.hpp"
#include "generator1.hpp"
#include "cat_generator3.hpp"
#include "skew_tent_generator1.hpp"
#include "ruleT_generator3.hpp"

template <size_t spectrum>
class iesidcm_cipher: public image_cipher_base<spectrum> {

private:
  static uint32_t discretize_(const double& value);

  static uint32_t* permutation_(uint32_t size, generator3* mapper);
  static uint32_t* inversate_(uint32_t* permutation, uint32_t size);

protected:
  static uint8_t* shuffle_(uint8_t* pixels, uint32_t size, generator3* mapper);
  static uint8_t* unshuffle_(uint8_t* pixels, uint32_t size, generator3* mapper);

  static uint8_t* substitute_(uint8_t* pixels, uint32_t size, generator3* mapper, uint32_t block_size);
  static uint8_t* unsubstitute_(uint8_t* pixels, uint32_t size, generator3* mapper, uint32_t block_size);

public:
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const override;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const override;
};

template <size_t spectrum>
inline uint32_t iesidcm_cipher<spectrum>::discretize_(const double& value) {
  static double TEN_TO_FIFTEEN = std::pow(10.0, 15);
  return std::abs(std::floor(TEN_TO_FIFTEEN * value));
}

template <size_t spectrum>
inline uint32_t* iesidcm_cipher<spectrum>::permutation_(uint32_t size, generator3* mapper) {
  uint32_t* perm = new uint32_t[size];
  bool* has = new bool[size];
  memset(has, false, size * sizeof(bool));

  uint32_t max = size, i = 0;
  dvec3 p = mapper->current();

  uint32_t aux_x = 0, aux_y = 0, aux_z = 0;
  bool found = false;

  while (i < size) {
    found = false;

    aux_x = discretize_(p.x) % size;
    aux_y = discretize_(p.y) % size;
    aux_z = discretize_(p.z) % size;

    if (!has[aux_x]) {
      perm[i++] = aux_x, has[aux_x] = true;
      found = true;
    }
    if (!has[aux_y]) {
      perm[i++] = aux_y, has[aux_y] = true;
      found = true;
    }
    if (!has[aux_z]) {
      perm[i++] = aux_z, has[aux_z] = true;
      found = true;
    }

    if (!found) {
      uint32_t j = max - 1;
      for (; j > 0 && has[j]; j--);
      max = j;
      perm[i++] = max, has[max] = true;
    }

    p = mapper->next();
  }

  return perm;
}

template <size_t spectrum>
inline uint32_t* iesidcm_cipher<spectrum>::inversate_(uint32_t* permutation, uint32_t size) {
  uint32_t* inverse = new uint32_t[size];
  for (uint32_t i = 0; i < size; i++) {
    inverse[permutation[i]] = i;
  }
  return inverse;
}

template <size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::shuffle_(uint8_t* pixels, uint32_t size, generator3* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* shuffled = new uint8_t[size];
  uint32_t* permutation = permutation_(size, mapper);

  for (uint32_t idx = 0; idx < size; idx++) {
    shuffled[permutation[idx]] = pixels[idx];
  }

  delete[] permutation;

  return shuffled;
}

template <size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::unshuffle_(uint8_t* pixels, uint32_t size, generator3* mapper) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  uint8_t* unshuffled = new uint8_t[size];

  uint32_t* permutation = permutation_(size, mapper);
  uint32_t* inversePerm = inversate_(permutation, size);

  for (uint32_t idx = 0; idx < size; idx++) {
    unshuffled[inversePerm[idx]] = pixels[idx];
  }

  delete[] permutation, delete[] inversePerm;

  return unshuffled;
}

template<size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::substitute_(uint8_t* pixels, uint32_t size, generator3* mapper, uint32_t block_size) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  // Init IV
  uint8_t* iv = new uint8_t[block_size];
  for (size_t i = 0; i < block_size; i++) {
    iv[i] = uint8_t(10 * i + 2);
  }

  // Init encrypted buffer
  uint8_t* encrypted = new uint8_t[size];

  // Init algorithm declarations
  uint8_t u = 0;
  uint8_t* prev_block = &iv[0];
  uint8_t* rand_block = new uint8_t[block_size];

  for (uint32_t b = 0; b < size; b += block_size) {

    uint32_t w = 0;
    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      w += prev_block[p];
    }

    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      const dvec3 point = mapper->next();
      u += w + std::floor(256.0 * (point.x + point.y + point.z));
      rand_block[p] = u;
    }

    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      encrypted[b + p] = pixels[b + p] ^ rand_block[p];
    }

    const uint32_t t = 3 + w % 23;
    for (uint32_t j = 0; j < t; j++) { mapper->next(); }

    prev_block = encrypted + b;
  }

  delete[] rand_block, delete[] iv;

  return encrypted;
}

template<size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::unsubstitute_(uint8_t* pixels, uint32_t size, generator3* mapper, uint32_t block_size) {
  if (size % spectrum != 0) {
    throw std::invalid_argument("Size must be a multiple of spectrum");
  }

  // Init IV
  uint8_t* iv = new uint8_t[block_size];
  for (size_t i = 0; i < block_size; i++) {
    iv[i] = uint8_t(10 * i + 2);
  }

  // Init decrypted buffer
  uint8_t* decrypted = new uint8_t[size];

  // Init algorithm declarations
  uint8_t u = 0;
  uint8_t* prev_block = &iv[0];
  uint8_t* rand_block = new uint8_t[block_size];

  for (uint32_t b = 0; b < size; b += block_size) {

    uint32_t w = 0;
    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      w += prev_block[p];
    }

    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      const dvec3 point = mapper->next();
      u += w + std::floor(256.0 * (point.x + point.y + point.z));
      rand_block[p] = u;
    }

    for (uint32_t p = 0; p < block_size && p + b < size; p++) {
      decrypted[b + p] = pixels[b + p] ^ rand_block[p];
    }

    const uint32_t t = 3 + w % 23;
    for (uint32_t j = 0; j < t; j++) { mapper->next(); }

    prev_block = pixels + b;
  }

  delete[] rand_block, delete[] iv;

  return decrypted;
}

template<size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {

  skew_tent_generator1 skew_tent_map1(0.42377183397545126, 0.9211909680024992);
  skew_tent_generator1 skew_tent_map2(0.45987045954620664, 0.39736630183773136);
  skew_tent_generator1 skew_tent_map3(0.8832720468837092, 0.2091818714907403);

  ruleT_generator3 ruleT_map(skew_tent_map1, skew_tent_map2, skew_tent_map3);
  cat_generator3 cat_map(0.237218932182183, 0.7213872103872803217, 0.346426821827137912, 2, 2, 2, 1, 1, 1);

  uint8_t* shuffled = shuffle_(pixels, size, &cat_map);
  uint8_t* substituted = substitute_(shuffled, size, &ruleT_map, 32);

  delete[] shuffled;
  return substituted;
}

template<size_t spectrum>
inline uint8_t* iesidcm_cipher<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {

  skew_tent_generator1 skew_tent_map1(0.42377183397545126, 0.9211909680024992);
  skew_tent_generator1 skew_tent_map2(0.45987045954620664, 0.39736630183773136);
  skew_tent_generator1 skew_tent_map3(0.8832720468837092, 0.2091818714907403);

  ruleT_generator3 ruleT_map(skew_tent_map1, skew_tent_map2, skew_tent_map3);
  cat_generator3 cat_map(0.237218932182183, 0.7213872103872803217, 0.346426821827137912, 2, 2, 2, 1, 1, 1);

  uint8_t* unsubstituted = unsubstitute_(pixels, size, &ruleT_map, 32);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, &cat_map);

  delete[] unsubstituted;
  return unshuffled;
}
