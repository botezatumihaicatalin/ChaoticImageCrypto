#pragma once

#include "spic.hpp"
#include "generator2.hpp"
#include "serpentine2_generator2.hpp"
#include "serpentine_spic_key.hpp"

template <size_t spectrum>
class serpentine2_spic : public spic<spectrum> {

private:
  serpentine_spic_key* key_;

public:
  serpentine2_spic() : key_(nullptr) {}

  void init_key(const serpentine_spic_key& key);
  uint8_t* encrypt(uint8_t* pixels, uint32_t size) const override;
  uint8_t* decrypt(uint8_t* pixels, uint32_t size) const override;
};

template <size_t spectrum>
void serpentine2_spic<spectrum>::init_key(const serpentine_spic_key& key) {
  key_ = new serpentine_spic_key(key);
}

template <size_t spectrum>
inline uint8_t* serpentine2_spic<spectrum>::encrypt(uint8_t* pixels, uint32_t size) const {
  if (key_ == nullptr) {
    throw std::invalid_argument("You need to call init_key before calling encrypt.");
  }

  serpentine2_generator2 mapper1(key_->p1(), key_->r1());
  serpentine2_generator2 mapper2(key_->p2(), key_->r2());

  mapper1.next_n(key_->m1()), mapper2.next_n(key_->m2());

  return this->do_encryption_(pixels, size, &mapper1, &mapper2, key_->iv());
}

template <size_t spectrum>
inline uint8_t* serpentine2_spic<spectrum>::decrypt(uint8_t* pixels, uint32_t size) const {
  if (key_ == nullptr) {
    throw std::invalid_argument("You need to call init_key before calling decrypt.");
  }

  serpentine2_generator2 mapper1(key_->p1(), key_->r1());
  serpentine2_generator2 mapper2(key_->p2(), key_->r2());

  mapper1.next_n(key_->m1()), mapper2.next_n(key_->m2());

  return this->do_decryption_(pixels, size, &mapper1, &mapper2, key_->iv());
}
