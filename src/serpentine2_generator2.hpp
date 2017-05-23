#pragma once

#include <cmath>
#include "generator2.hpp"
#include "luts_math.hpp"

// Same as serpentine_generator2, but with LUTS
class serpentine2_generator2 : public generator2 {

protected:
  double pow_2_exponent_;
  double exponent_;

public:
  serpentine2_generator2(const dvec2& start, const double& exponent) :
    generator2(start), pow_2_exponent_(pow(2, exponent)), exponent_(exponent) {}

  serpentine2_generator2(const double& x, const double& y, const double& exponent) :
    generator2(x, y), pow_2_exponent_(pow(2, exponent)), exponent_(exponent) {}

  const dvec2& next() override;
  const double& exponent() const;
};

inline const dvec2& serpentine2_generator2::next() {
  const double exp_x = pow_2_exponent_ * current_.x;
  const double exp_y = pow_2_exponent_ * current_.y;
  current_.x = std::atan(1.0 / lmath::tan(exp_x));
  current_.y = lmath::sin(exp_y) * lmath::cos(exp_y);
  return current_;
}

inline const double& serpentine2_generator2::exponent() const {
  return exponent_;
}
