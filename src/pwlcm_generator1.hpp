#pragma once

#include "generator1.hpp"

class pwlcm_generator1 : public generator1 {

protected:
  double exponent_;

public:
  pwlcm_generator1(const double& start, const double& exponent)
    : generator1(start), exponent_(exponent) {}

  const double& next() override;
  const double& exponent() const;

private:
  double next_pwlcm(const double& current) const;
};

inline double pwlcm_generator1::next_pwlcm(const double& current) const {
  if (current >= 0 && current <= exponent_) {
    return current / exponent_;
  }
  if (current >= exponent_ && current < 0.5) {
    return (current - exponent_) / (0.5 - exponent_);
  }

  return next_pwlcm(1.0 - current);
}

inline const double& pwlcm_generator1::next() {
  current_ = next_pwlcm(current_);
  return current_;
}

inline const double& pwlcm_generator1::exponent() const {
  return exponent_;
}
