#pragma once

#include "generator.hpp"

class chaotic_generator: public generator {
  
protected:
  double exponent_;

public:
  chaotic_generator(const dvec2& start, const double& exponent) :
    generator(start), exponent_(exponent) {}
};