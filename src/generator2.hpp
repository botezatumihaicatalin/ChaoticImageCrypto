#pragma once

#include "./dvec2.hpp"

class generator2 {

protected:
  dvec2 current_;
  dvec2 start_;

public:
  explicit generator2(dvec2 start) : current_(start), start_(start) {}
  explicit generator2(const double& x, const double& y) : generator2(dvec2(x, y)) {};

  virtual ~generator2() = default;
  virtual const dvec2& next() = 0;

  const dvec2& next_n(size_t steps);
  const dvec2& current() const;
  const dvec2& start() const;
};

inline const dvec2& generator2::next_n(size_t steps) {
  size_t i = 0;
  for (i = 0; i < steps; i++) {
    next();
  }
  return current_;
}

inline const dvec2& generator2::current() const {
  return current_;
}

inline const dvec2& generator2::start() const {
  return start_;
}
