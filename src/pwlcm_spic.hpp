#pragma once

#pragma once

#include "spic.hpp"
#include "generator2.hpp"
#include "pwlcm_generator2.hpp"

template <size_t spectrum>
class pwlcm_spic : protected spic<spectrum> {

public:
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const override;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const override;
};

template <size_t spectrum>
inline uint8_t* pwlcm_spic<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 0.2, r2 = 0.3;
  double x01 = 0.1567, y01 = 0.3219;
  double x02 = 0.4567, y02 = 0.1111;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator2* mapper1 = new pwlcm_generator2(dvec2(x01, y01), r1);
  generator2* mapper2 = new pwlcm_generator2(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* shuffled = shuffle_(pixels, size, mapper1);
  uint8_t* substituted = substitute_(shuffled, size, mapper2, iv);

  delete mapper1 , delete mapper2;
  delete[] shuffled;

  return substituted;
}

template <size_t spectrum>
inline uint8_t* pwlcm_spic<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {
  double r1 = 0.2, r2 = 0.3;
  double x01 = 0.1567, y01 = 0.3219;
  double x02 = 0.4567, y02 = 0.1111;
  uint32_t m1 = 2017, m2 = 2016;
  uint32_t iv = 123456;

  generator2* mapper1 = new pwlcm_generator2(dvec2(x01, y01), r1);
  generator2* mapper2 = new pwlcm_generator2(dvec2(x02, y02), r2);

  mapper1->next_n(m1) , mapper2->next_n(m2);

  uint8_t* unsubstituted = unsubstitute_(pixels, size, mapper2, iv);
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, mapper1);

  delete mapper1 , delete mapper2;
  delete[] unsubstituted;

  return unshuffled;
}
