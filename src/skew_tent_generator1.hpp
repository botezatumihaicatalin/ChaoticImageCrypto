#pragma once
#pragma once

#include "generator1.hpp"

class skew_tent_generator1 : public generator1 {

protected:
  double exponent_;

public:
  skew_tent_generator1(const double& start, const double& exponent)
    : generator1(start), exponent_(exponent) {}

  const double& next() override;
  const double& exponent() const;
};

inline const double& skew_tent_generator1::next() {
  if (current_ <= exponent_ && current_ >= 0) {
    current_ = current_ / exponent_;
  }
  else if (current_ <= 1 && current_ > exponent_) {
    current_ = (1 - current_) / (1 - exponent_);
  }
  return current_;
}

inline const double& skew_tent_generator1::exponent() const {
  return exponent_;
}
