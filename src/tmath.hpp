#pragma once

#include <cmath>

#define MAX_CIRCLE_ANGLE      512
#define HALF_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/2)
#define QUARTER_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/4)
#define MASK_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE - 1)
#define T_PI 3.14159265358979323846

float fast_cossin_table[MAX_CIRCLE_ANGLE];

struct table_filler {
  table_filler() {
    for (size_t i = 0; i < MAX_CIRCLE_ANGLE; i++) {
      fast_cossin_table[i] = std::sin(i * T_PI / HALF_MAX_CIRCLE_ANGLE);
    }
  }
} table_filler_instance;

inline double tcos(double n) {
  int i = int(n * HALF_MAX_CIRCLE_ANGLE / T_PI);
  if (i < 0) {
    return fast_cossin_table[((-i) + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE];
  }
  return fast_cossin_table[(i + QUARTER_MAX_CIRCLE_ANGLE) & MASK_MAX_CIRCLE_ANGLE];
}

inline double tsin(double n) {
  int i = int(n * HALF_MAX_CIRCLE_ANGLE / T_PI);
  if (i < 0) {
    return fast_cossin_table[(-((-i) & MASK_MAX_CIRCLE_ANGLE)) + MAX_CIRCLE_ANGLE];
  }
  return fast_cossin_table[i & MASK_MAX_CIRCLE_ANGLE];
}

inline double ttan(double n) {
  return tsin(n) / tcos(n);
}

inline double tctg(double n) {
  return tcos(n) / tsin(n);
}
