#pragma once

#include "chaotic_generator.hpp"
#include <cmath>

class serpentine_generator: public chaotic_generator {

protected:
  double pow_2_exponent_;

public:
  serpentine_generator(const dvec2& start, const double& exponent) :
    chaotic_generator(start, exponent), pow_2_exponent_(pow(2, exponent)) {};
  serpentine_generator(const double& x, const double& y, const double& exponent) :
    chaotic_generator(x, y, exponent), pow_2_exponent_(pow(2, exponent)) {};
  const dvec2& next() override;
};

inline const dvec2& serpentine_generator::next() {
  const double exp_x = pow_2_exponent_ * current_.x;
  const double exp_y = pow_2_exponent_ * current_.y;
  current_.x = std::atan(1 / std::tan(exp_x));
  current_.y = std::sin(exp_y) * std::cos(exp_y);
  return current_;
}
