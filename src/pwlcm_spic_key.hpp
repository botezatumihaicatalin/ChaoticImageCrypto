#pragma once

#include "dvec2.hpp"
#include <stdint.h>
#include "spic_key.hpp"

class pwlcm_spic_key : public spic_key {

private:
  double r1_, r2_;
  dvec2 p1_, p2_;
  uint32_t m1_, m2_;
  uint32_t iv_;

public:
  pwlcm_spic_key(const dvec2& p1, const dvec2& p2, const double& r1, const double& r2,
                 const uint32_t& m1, const uint32_t& m2, const uint32_t& iv) :
    spic_key(), r1_(r1), r2_(r2), p1_(p1), p2_(p2), m1_(m1), m2_(m2), iv_(iv) {}

  const double& r1() const;
  const double& r2() const;
  const dvec2& p1() const;
  const dvec2& p2() const;
  const uint32_t& m1() const;
  const uint32_t& m2() const;
  const uint32_t& iv() const;
};

inline const double& pwlcm_spic_key::r1() const {
  return r1_;
}

inline const double& pwlcm_spic_key::r2() const {
  return r2_;
}

inline const dvec2& pwlcm_spic_key::p1() const {
  return p1_;
}

inline const dvec2& pwlcm_spic_key::p2() const {
  return p2_;
}

inline const uint32_t& pwlcm_spic_key::m1() const {
  return m1_;
}

inline const uint32_t& pwlcm_spic_key::m2() const {
  return m2_;
}

inline const uint32_t& pwlcm_spic_key::iv() const {
  return iv_;
}
