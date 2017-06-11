#pragma once

#include <cmath>
#include "generator3.hpp"
#include "skew_tent_generator1.hpp"

// Composed of three skew_tent maps.
class ruleT_generator3 : public generator3 {

protected:
  skew_tent_generator1 skew_tent1_;
  skew_tent_generator1 skew_tent2_;
  skew_tent_generator1 skew_tent3_;

public:
  ruleT_generator3(const dvec3& start, const double& exp1, const double& exp2, const double& exp3)
    : generator3(start), skew_tent1_(start.x, exp1), skew_tent2_(start.y, exp2), skew_tent3_(start.z, exp3) {}

  ruleT_generator3(const skew_tent_generator1& gen1, const skew_tent_generator1& gen2, const skew_tent_generator1& gen3)
    : generator3(gen1.current(), gen2.current(), gen3.current()), skew_tent1_(gen1), skew_tent2_(gen2), skew_tent3_(gen3) {}

  ruleT_generator3(const double& x, const double& y, const double& z, 
                   const double& exp1, const double& exp2, const double& exp3)
    : generator3(x, y, z), skew_tent1_(x, exp1), skew_tent2_(y, exp2), skew_tent3_(z, exp3) {}

  const dvec3& next() override;
};

inline const dvec3& ruleT_generator3::next() {
  current_.x = skew_tent1_.next();
  if (current_.x < 0.5) {
    current_.y = skew_tent2_.next();
  }
  else {
    current_.z = skew_tent3_.next();
  }
  return current_;
}

