#pragma once

#include "generator2.hpp"
#include "pwlcm_generator1.hpp"

class pwlcm_generator2 : public generator2 {

protected:
  pwlcm_generator1 generator1_;
  pwlcm_generator1 generator2_;
   
public:
  pwlcm_generator2(const dvec2& start, const double& exponent)
    : generator2(start), generator1_(start.x, exponent), generator2_(start.y, exponent) {}

  pwlcm_generator2(const pwlcm_generator1& gen1, const pwlcm_generator1& gen2)
    : generator2(gen1.current(), gen2.current()), generator1_(gen1), generator2_(gen2) {}

  pwlcm_generator2(const double& x, const double& y, const double& exponent)
    : generator2(x, y), generator1_(x, exponent), generator2_(y, exponent) {}

  const dvec2& next() override;
  const double& exponent() const;
};

inline const dvec2& pwlcm_generator2::next() {
  current_.x = generator1_.next();
  current_.y = generator2_.next();
  return current_;
}

inline const double & pwlcm_generator2::exponent() const {
  return generator1_.exponent();
}
