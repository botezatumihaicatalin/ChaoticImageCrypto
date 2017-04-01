#pragma once

#include "./dvec2.hpp"

class generator {

protected:
  dvec2 current_;
  dvec2 start_;

public:
  explicit generator(dvec2 start) : current_(start), start_(start) {}
  explicit generator(const double& x, const double& y) : generator(dvec2(x, y)) {};

  virtual ~generator() = default;
  virtual const dvec2& next() = 0;

  const dvec2& next_n(size_t steps);
  const dvec2& current() const;
  const dvec2& start() const;
};

inline const dvec2& generator::next_n(size_t steps) {
  size_t i = 0;
  for (i = 0; i < steps; i++) {
    next();
  }
  return current_;
}

inline const dvec2& generator::current() const {
  return current_;
}

inline const dvec2& generator::start() const {
  return start_;
}
