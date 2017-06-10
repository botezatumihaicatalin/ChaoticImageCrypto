#pragma once

#include "./dvec3.hpp"

class generator3 {

protected:
  dvec3 current_;
  dvec3 start_;

public:
  explicit generator3(dvec3 start) : current_(start), start_(start) {}
  explicit generator3(const double& x, const double& y, const double& z) : generator3(dvec3(x, y, z)) {};

  virtual ~generator3() = default;
  virtual const dvec3& next() = 0;

  const dvec3& next_n(size_t steps);
  const dvec3& current() const;
  const dvec3& start() const;
};

inline const dvec3& generator3::next_n(size_t steps) {
  size_t i = 0;
  for (i = 0; i < steps; i++) {
    next();
  }
  return current_;
}

inline const dvec3& generator3::current() const {
  return current_;
}

inline const dvec3& generator3::start() const {
  return start_;
}
