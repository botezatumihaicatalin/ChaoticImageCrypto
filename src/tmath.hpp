#pragma once

#include <cmath>

#define MAX_CIRCLE_ANGLE      512
#define HALF_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/2)
#define QUARTER_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/4)
#define MASK_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE - 1)
#define T_PI 3.14159265358979323846

double fast_cossin_table[MAX_CIRCLE_ANGLE];

struct table_filler {
  table_filler() {
    for (size_t i = 0; i < MAX_CIRCLE_ANGLE; i++) {
      fast_cossin_table[i] = std::sin(i * T_PI / HALF_MAX_CIRCLE_ANGLE);
    }
  }
} table_filler_instance;

inline double tcos(double n) {
  double f = n * HALF_MAX_CIRCLE_ANGLE / T_PI;
  int i = int(f); double d = f - i;
  if (i < 0) {
    i = ((-i) + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE;
  }
  else {
    i = (i + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE;
  }
  double cos1 = fast_cossin_table[i];
  double cos2 = fast_cossin_table[i + 1];
  return cos1 + (cos2 - cos1) * d;
}

inline double tsin(double n) {
  double f = n * HALF_MAX_CIRCLE_ANGLE / T_PI;
  int i = int(f); double d = f - i;
  if (i < 0) {
    i = (-((-i) & MASK_MAX_CIRCLE_ANGLE)) + MAX_CIRCLE_ANGLE;
  }
  else {
    i = i & MASK_MAX_CIRCLE_ANGLE;
  }
  double sin1 = fast_cossin_table[i];
  double sin2 = fast_cossin_table[i + 1];
  return sin1 + (sin2 - sin1) * d;
}

inline double ttan(double n) {
  return tsin(n) / tcos(n);
}

inline double tctg(double n) {
  return tcos(n) / tsin(n);
}
