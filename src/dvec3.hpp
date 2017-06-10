#pragma once

class dvec3 {

public:
  double x;
  double y;
  double z;

  dvec3(const double& value) : dvec3(value, value, value) {};
  dvec3(const double& x, const double& y, const double& z) : x(x), y(y), z(z) {};
};
