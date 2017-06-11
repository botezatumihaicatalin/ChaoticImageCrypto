#pragma once

#include <stdint.h>
#include <stdexcept>

#include "generator3.hpp"
#include "generator1.hpp"
#include "cat_generator3.hpp"
#include "skew_tent_generator1.hpp"
#include "ruleT_generator3.hpp"

template <size_t spectrum> class iesidcm {

private:
  static size_t const pixel_size;

  static uint32_t discretize_(const double& value);

  static uint32_t* permutation_(uint32_t size, generator3* mapper);
  static uint32_t* inversate_(uint32_t* permutation, uint32_t size);

protected:
  static uint8_t* shuffle_(uint8_t* pixels, uint32_t size, generator3* mapper);
  static uint8_t* unshuffle_(uint8_t* pixels, uint32_t size, generator3* mapper);

  static uint8_t* substitute_(uint8_t* pixels, uint32_t size, generator3* mapper, size_t block_size);
  static uint8_t* unsubstitute_(uint8_t* pixels, uint32_t size, generator3* mapper, size_t block_size);

public:
  virtual ~iesidcm() = default;
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const;
};

template <size_t spectrum>
size_t const iesidcm<spectrum>::pixel_size = spectrum * sizeof(uint8_t);

template <size_t spectrum>
inline uint32_t iesidcm<spectrum>::discretize_(const double& value) {
  static double TEN_TO_FIFTEEN = std::pow(10.0, 15);
  return std::abs(std::floor(TEN_TO_FIFTEEN * value));
}

template <size_t spectrum>
inline uint32_t* iesidcm<spectrum>::permutation_(uint32_t size, generator3* mapper) {
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
inline uint32_t* iesidcm<spectrum>::inversate_(uint32_t* permutation, uint32_t size) {
  uint32_t* inverse = new uint32_t[size];
  for (uint32_t i = 0; i < size; i++) { 
    inverse[permutation[i]] = i; 
  }
  return inverse;
}

template <size_t spectrum>
inline uint8_t* iesidcm<spectrum>::shuffle_(uint8_t* pixels, uint32_t size, generator3* mapper) {
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
inline uint8_t* iesidcm<spectrum>::unshuffle_(uint8_t* pixels, uint32_t size, generator3* mapper) {
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
inline uint8_t* iesidcm<spectrum>::substitute_(uint8_t* pixels, uint32_t size, generator3* mapper, size_t block_size) {
  uint8_t* output = new uint8_t[size];
  memcpy(output, pixels, size * sizeof(uint8_t));
  return output;
}

template<size_t spectrum>
inline uint8_t* iesidcm<spectrum>::unsubstitute_(uint8_t* pixels, uint32_t size, generator3* mapper, size_t block_size) {
  uint8_t* output = new uint8_t[size];
  memcpy(output, pixels, size * sizeof(uint8_t));
  return output;
}

template<size_t spectrum>
inline uint8_t * iesidcm<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {

  skew_tent_generator1 skew_tent1(0.42377183397545126, 0.9211909680024992);
  skew_tent_generator1 skew_tent2(0.45987045954620664, 0.39736630183773136);
  skew_tent_generator1 skew_tent3(0.8832720468837092, 0.2091818714907403);

  ruleT_generator3 ruleT_map(skew_tent1, skew_tent2, skew_tent3);
  cat_generator3 cat_map(0.237218932182183, 0.7213872103872803217, 0.346426821827137912, 2, 2, 2, 1, 1, 1);

  uint8_t* shuffled = shuffle_(pixels, size, &cat_map);
  uint8_t* substituted = substitute_(shuffled, size, &ruleT_map, 32);

  delete[] shuffled;
  return substituted;
}

template<size_t spectrum>
inline uint8_t * iesidcm<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {

  skew_tent_generator1 skew_tent1(0.42377183397545126, 0.9211909680024992);
  skew_tent_generator1 skew_tent2(0.45987045954620664, 0.39736630183773136);
  skew_tent_generator1 skew_tent3(0.8832720468837092, 0.2091818714907403);

  ruleT_generator3 ruleT_map(skew_tent1, skew_tent2, skew_tent3);
  cat_generator3 cat_map(0.237218932182183, 0.7213872103872803217, 0.346426821827137912, 2, 2, 2, 1, 1, 1);

  uint8_t* unsubstituted = unsubstitute_(pixels, size, &ruleT_map, 32);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, &cat_map);

  delete[] unsubstituted;
  return unshuffled;
}
