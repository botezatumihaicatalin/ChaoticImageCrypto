#pragma once

#include "dvec2.hpp"
#include <stdint.h>

class spic_key {

private:
  dvec2 p1_, p2_;
  uint32_t m1_, m2_;
  uint32_t iv_;

public:
  spic_key(const dvec2& p1, const dvec2& p2, const uint32_t& m1, const uint32_t& m2, const uint32_t& iv) :
    p1_(p1), p2_(p2), m1_(m1), m2_(m2), iv_(iv) {}

  const dvec2& p1() const;
  const dvec2& p2() const;
  const uint32_t& m1() const;
  const uint32_t& m2() const;
  const dvec2& iv() const;

};

inline const dvec2& spic_key::p1() const {
  return p1_;
}

inline const dvec2& spic_key::p2() const {
  return p2_;
}

inline const uint32_t& spic_key::m1() const {
  return m1_;
}

inline const uint32_t& spic_key::m2() const {
  return m2_;
}

inline const dvec2& spic_key::iv() const {
  return iv_;
}
