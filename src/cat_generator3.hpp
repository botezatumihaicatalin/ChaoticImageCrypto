#pragma once

#include <cmath>
#include "generator3.hpp"

class cat_generator3 : public generator3 {

protected:
  double ax_, ay_, az_;
  double bx_, by_, bz_;
  double matrix_[3][3];

public:
  cat_generator3(const dvec3& start, const double& ax, const double& ay,
                 const double& az, const double& bx, const double& by, const double& bz);

  cat_generator3(const double& x, const double& y, const double& z, 
                 const double& ax, const double& ay, const double& az, 
                 const double& bx, const double& by, const double& bz):
    cat_generator3(dvec3(x, y, z), ax, ay, az, bx, by, bz) { }

  const dvec3& next() override;
};

inline cat_generator3::cat_generator3(const dvec3& start, const double& ax, const double& ay,
                                      const double& az, const double& bx, const double& by, const double& bz) :
  generator3(start), ax_(ax), ay_(ay), az_(az), bx_(bx), by_(by), bz_(bz) {

  matrix_[0][0] = 1.0 + ax * az * by;
  matrix_[0][1] = az;
  matrix_[0][2] = ay + ax*az + ax*ay*az*by;
  matrix_[1][0] = bz + ax*by + ax*az*by*bz;
  matrix_[1][1] = az*bz + 1;
  matrix_[1][2] = ay*bz + ax*ay*az*by*bz + ax*az*bz + ax*ay*by + ax;
  matrix_[2][0] = ax*bx*by + by;
  matrix_[2][1] = bx;
  matrix_[2][2] = ax*ay*bx*by + ax*bx + ay*by + 1.0;
}

inline const dvec3& cat_generator3::next() {
  double remainder;
  const double new_x = std::modf(matrix_[0][0] * current_.x + matrix_[0][1] * current_.y + matrix_[0][2] * current_.z, &remainder);
  const double new_y = std::modf(matrix_[1][0] * current_.x + matrix_[1][1] * current_.y + matrix_[1][2] * current_.z, &remainder);
  const double new_z = std::modf(matrix_[2][0] * current_.x + matrix_[2][1] * current_.y + matrix_[2][2] * current_.z, &remainder);
  current_.x = new_x, current_.y = new_y, current_.z = new_z;
  return current_;
}

