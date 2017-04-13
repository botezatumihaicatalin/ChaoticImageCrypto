#pragma once

#include "generator2.hpp"

class pwlcm_generator2 : public generator2 {

protected:
  double exponent_;

public:
  pwlcm_generator2(const dvec2& start, const double& exponent)
    : generator2(start), exponent_(exponent) {}

  pwlcm_generator2(const double& x, const double& y, const double& exponent)
    : generator2(x, y), exponent_(exponent) {}

  const dvec2& next() override;

private:
  static double next_pwlcm(const double& current, const double& exp);
};

inline double pwlcm_generator2::next_pwlcm(const double& current, const double& exp) {
  if (current >= 0 && current <= exp) {
    return current / exp;
  }
  if (current >= exp && current < 0.5) {
    return (current - exp) / (0.5 - exp);
  }

  return next_pwlcm(1.0 - current, exp);
}

inline const dvec2& pwlcm_generator2::next() {
  current_.x = next_pwlcm(current_.x, exponent_);
  current_.y = next_pwlcm(current_.y, exponent_);
  return current_;
}
