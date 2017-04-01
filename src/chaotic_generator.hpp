#pragma once

#include "generator.hpp"

class chaotic_generator: public generator {

protected:
  double exponent_;

public:
  chaotic_generator(const dvec2& start, const double& exponent) :
    generator(start), exponent_(exponent) {}
  chaotic_generator(const double& x, const double& y, const double& exponent) :
    generator(x, y), exponent_(exponent) {}
};
