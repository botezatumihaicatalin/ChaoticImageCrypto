#pragma once

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "spic.hpp"
#include "generator2.hpp"
#include "serpentine_generator2.hpp"

template <size_t spectrum>
class serpentine_spic: protected spic<spectrum> {

public:
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const override;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const override;
};

template <size_t spectrum>
inline uint8_t* serpentine_spic<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 10.0, r2 = 20.0;
  double x01 = M_PI / 4.0, y01 = 1.0 / 4.0;
  double x02 = -M_PI / 4.0, y02 = -1 / 20.0;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator2* mapper1 = new serpentine_generator2(dvec2(x01, y01), r1);
  generator2* mapper2 = new serpentine_generator2(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* shuffled = shuffle_(pixels, size, mapper1);
  uint8_t* substituted = substitute_(shuffled, size, mapper2, iv);

  delete mapper1 , delete mapper2;
  delete[] shuffled;

  return substituted;
}

template <size_t spectrum>
inline uint8_t* serpentine_spic<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 10.0, r2 = 20.0;
  double x01 = M_PI / 4.0, y01 = 1.0 / 4.0;
  double x02 = -M_PI / 4.0, y02 = -1 / 20.0;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator2* mapper1 = new serpentine_generator2(dvec2(x01, y01), r1);
  generator2* mapper2 = new serpentine_generator2(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* unsubstituted = unsubstitute_(pixels, size, mapper2, iv);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, mapper1);

  delete mapper1 , delete mapper2;
  delete[] unsubstituted;

  return unshuffled;
}
