#pragma once

class dvec2 {

public:
  double x;
  double y;

  dvec2(const double& value) : dvec2(value, value) {};
  dvec2(const double& x, const double& y) : x(x), y(y) {};
};
