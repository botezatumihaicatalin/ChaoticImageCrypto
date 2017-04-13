#pragma once

class generator1 {

protected:
  double current_;
  double start_;

public:
  explicit generator1(const double& start) : current_(start), start_(start) {};

  virtual ~generator1() = default;
  virtual const double& next() = 0;

  const double& next_n(size_t steps);
  const double& current() const;
  const double& start() const;
};

inline const double& generator1::next_n(size_t steps) {
  size_t i = 0;
  for (i = 0; i < steps; i++) {
    next();
  }
  return current_;
}

inline const double& generator1::current() const {
  return current_;
}

inline const double& generator1::start() const {
  return start_;
}
