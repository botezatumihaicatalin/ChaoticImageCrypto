#pragma once

#include <cmath>

#define MAX_CIRCLE_ANGLE      65536
#define HALF_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/2)
#define QUARTER_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/4)
#define MASK_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE - 1)
#define T_PI 3.14159265358979323846

double fast_sin_table[MAX_CIRCLE_ANGLE];
double fast_cos_table[MAX_CIRCLE_ANGLE];

struct table_filler {
  table_filler() {
    for (size_t i = 0; i < MAX_CIRCLE_ANGLE; i++) {
      fast_sin_table[i] = std::sin(i * T_PI / HALF_MAX_CIRCLE_ANGLE);
      fast_cos_table[i] = std::cos(i * T_PI / HALF_MAX_CIRCLE_ANGLE);
    }
  }
} table_filler_instance;

inline double tcos(double n) {
  double f = n * HALF_MAX_CIRCLE_ANGLE / T_PI;
  int i1 = int(f) & MASK_MAX_CIRCLE_ANGLE;
  int i2 = (i1 + 1) & MASK_MAX_CIRCLE_ANGLE;
  double cos1 = fast_cos_table[i1];
  double cos2 = fast_cos_table[i2];
  return cos1 + (cos2 - cos1) * (f - i1);
}

inline double tsin(double n) {
  double f = n * HALF_MAX_CIRCLE_ANGLE / T_PI;
  int i1 = int(f) & MASK_MAX_CIRCLE_ANGLE;
  int i2 = (i1 + 1) & MASK_MAX_CIRCLE_ANGLE;
  double sin1 = fast_sin_table[i1];
  double sin2 = fast_sin_table[i2];
  return sin1 + (sin2 - sin1) * (f - i1);
}

inline double ttan(double n) {
  return tsin(n) / tcos(n);
}

inline double tctg(double n) {
  return tcos(n) / tsin(n);
}
