#pragma once

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "spic.hpp"
#include "generator2.hpp"
#include "serpentine_generator2.hpp"
#include "serpentine_spic_key.hpp"

template <size_t spectrum>
class serpentine_spic: protected spic<spectrum> {

private:
  serpentine_spic_key* key_;

public:
  serpentine_spic() : key_(nullptr) {}

  void init_key(spic_key* key) override;
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const override;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const override;
};

template<size_t spectrum>
inline void serpentine_spic<spectrum>::init_key(spic_key * key) {
  auto key_casted = dynamic_cast<serpentine_spic_key*>(key);
  key_ = new serpentine_spic_key(*key_casted);
}

template <size_t spectrum>
inline uint8_t* serpentine_spic<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {
  if (key_ == nullptr) {
    throw std::invalid_argument("You need to call init_key before calling encrypt.");
  }

  generator2* mapper1 = new serpentine_generator2(key_->p1(), key_->r1());
  generator2* mapper2 = new serpentine_generator2(key_->p2(), key_->r2());

  mapper1->next_n(key_->m1()), mapper2->next_n(key_->m2());

  uint8_t* shuffled = shuffle_(pixels, size, mapper1);
  uint8_t* substituted = substitute_(shuffled, size, mapper2, key_->iv());

  delete mapper1, delete mapper2;
  delete[] shuffled;

  return substituted;
}

template <size_t spectrum>
inline uint8_t* serpentine_spic<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {
  if (key_ == nullptr) {
    throw std::invalid_argument("You need to call init_key before calling decrypt.");
  }

  generator2* mapper1 = new serpentine_generator2(key_->p1(), key_->r1());
  generator2* mapper2 = new serpentine_generator2(key_->p2(), key_->r2());

  mapper1->next_n(key_->m1()), mapper2->next_n(key_->m2());

  uint8_t* unsubstituted = unsubstitute_(pixels, size, mapper2, key_->iv());
  uint8_t* unshuffled = unshuffle_(unsubstituted, size, mapper1);

  delete mapper1, delete mapper2;
  delete[] unsubstituted;

  return unshuffled;
}
