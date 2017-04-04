#pragma once

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include <string.h>
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <iostream>
#include "generator.hpp"
#include "serpentine_generator.hpp"

class spic {

private:
  static uint32_t discretize(const double& value);
  static uint32_t* permutation_(uint32_t size, generator* mapper);
  static uint32_t* inversate_(uint32_t* permutation, uint32_t size);
  static uint8_t* shuffle_(uint8_t* pixels, uint32_t size, generator* mapper);
  static uint8_t* unshuffle_(uint8_t* pixels, uint32_t size, generator* mapper);
  static uint8_t* substitute_(uint8_t* pixels, uint32_t size, generator* mapper, uint32_t iv);
  static uint8_t* unsubstitute_(uint8_t* pixels, uint32_t size, generator* mapper, uint32_t iv);

public:
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const;
};

inline uint32_t spic::discretize(const double& value) {
  static double TEN_TO_FIFTEEN = std::pow(10.0, 15);
  return std::abs(std::floor(TEN_TO_FIFTEEN * value));
}

inline uint32_t* spic::permutation_(uint32_t size, generator* mapper) {
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

inline uint32_t* spic::inversate_(uint32_t* permutation, uint32_t size) {
  uint32_t* inverse = new uint32_t[size];
  for (uint32_t i = 0; i < size; i++) {
    inverse[permutation[i]] = i;
  }
  return inverse;
}

inline uint8_t* spic::shuffle_(uint8_t* pixels, uint32_t size, generator* mapper) {
  uint8_t* shuffled = new uint8_t[size];
  uint32_t* permutation = permutation_(size / 4, mapper);
  const size_t copy_size = 4 * sizeof(uint8_t);

  for (uint32_t i = 0; i < size / 4; i++) {
    memcpy(&shuffled[permutation[i] * 4], &pixels[i * 4], copy_size);
  }

  delete[] permutation;

  return shuffled;
}

inline uint8_t* spic::unshuffle_(uint8_t* pixels, uint32_t size, generator* mapper) {
  uint8_t* unshuffled = new uint8_t[size];

  uint32_t* permutation = permutation_(size / 4, mapper);
  uint32_t* inverse = inversate_(permutation, size / 4);

  size_t copy_size = 4 * sizeof(uint8_t);

  for (uint32_t i = 0; i < size / 4; i++) {
    memcpy(&unshuffled[inverse[i] * 4], &pixels[i * 4], copy_size);
  }

  delete[] permutation, delete[] inverse;

  return unshuffled;
}

inline uint8_t* spic::substitute_(uint8_t* pixels, uint32_t size, generator* mapper, uint32_t iv) {
  uint8_t* encrypted = new uint8_t[size];

  dvec2 p2 = mapper->current();
  uint32_t ks1 = discretize(p2.x);
  uint32_t ks2 = discretize(p2.y);

  uint32_t curr_pixel = 0;
  memcpy(&curr_pixel, &pixels[0], 4 * sizeof(uint8_t));

  uint32_t encr_pixel = iv ^ curr_pixel ^ ks1 ^ ks2;
  memcpy(&encrypted[0], &encr_pixel, 4 * sizeof(uint8_t));

  uint32_t prev_pixel = encr_pixel;

  uint32_t channels = 0;

  for (uint32_t i = 4; i < size; i += 4) {
    p2 = mapper->next();

    ks1 = discretize(p2.x);
    ks2 = discretize(p2.y);

    channels += encrypted[i - 4] + encrypted[i - 3] + encrypted[i - 2] + encrypted[i - 1];

    memcpy(&curr_pixel, &pixels[i], 4 * sizeof(uint8_t));
    encr_pixel = ((curr_pixel ^ ks1) + channels) ^ prev_pixel ^ ks2;
    prev_pixel = encr_pixel;

    memcpy(&encrypted[i], &encr_pixel, 4 * sizeof(uint8_t));
  }

  return encrypted;
}

inline uint8_t* spic::unsubstitute_(uint8_t* pixels, uint32_t size, generator* mapper, uint32_t iv) {
  uint8_t* decrypted = new uint8_t[size];

  dvec2 p2 = mapper->current();
  uint32_t ks1 = discretize(p2.x);
  uint32_t ks2 = discretize(p2.y);

  uint32_t curr_pixel = 0;
  memcpy(&curr_pixel, &pixels[0], 4 * sizeof(uint8_t));

  uint32_t prev_pixel = curr_pixel;

  uint32_t decr_pixel = iv ^ curr_pixel ^ ks1 ^ ks2;
  memcpy(&decrypted[0], &decr_pixel, 4 * sizeof(uint8_t));

  uint32_t channels = 0;

  for (uint32_t i = 4; i < size; i += 4) {
    p2 = mapper->next();

    ks1 = discretize(p2.x);
    ks2 = discretize(p2.y);

    channels += pixels[i - 4] + pixels[i - 3] + pixels[i - 2] + pixels[i - 1];

    memcpy(&curr_pixel, &pixels[i], 4 * sizeof(uint8_t));
    decr_pixel = ((prev_pixel ^ curr_pixel ^ ks2) - channels) ^ ks1;
    prev_pixel = curr_pixel;

    memcpy(&decrypted[i], &decr_pixel, 4 * sizeof(uint8_t));
  }

  return decrypted;
}

inline uint8_t* spic::encrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 10.0, r2 = 20.0;
  double x01 = M_PI / 4.0, y01 = 1.0 / 4.0;
  double x02 = -M_PI / 4.0, y02 = -1 / 20.0;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator* mapper1 = new serpentine_generator(dvec2(x01, y01), r1);
  generator* mapper2 = new serpentine_generator(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* shuffled = shuffle_(pixels, size, mapper1);
  uint8_t* substituted = substitute_(shuffled, size, mapper2, iv);

  delete mapper1, delete mapper2;
  delete[] shuffled;

  return substituted;
}

inline uint8_t* spic::decrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 10.0, r2 = 20.0;
  double x01 = M_PI / 4.0, y01 = 1.0 / 4.0;
  double x02 = -M_PI / 4.0, y02 = -1 / 20.0;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator* mapper1 = new serpentine_generator(dvec2(x01, y01), r1);
  generator* mapper2 = new serpentine_generator(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* unsubstituted = unsubstitute_(pixels, size, mapper2, iv);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, mapper1);

  delete mapper1, delete mapper2;
  delete[] unsubstituted;

  return unshuffled;
}
