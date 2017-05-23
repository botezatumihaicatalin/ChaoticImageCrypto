#pragma once

#include <cmath>
#include <math.h>

namespace lmath {
  
  const size_t MAX_RESOLUTION = 65536;
  const size_t HALF_MAX_RESOLUTION = MAX_RESOLUTION / 2;
  const size_t MASK_MAX_RESOLUTION = MAX_RESOLUTION - 1;
  const double L_PI = 3.14159265358979323846;

  double fast_sin_table[MAX_RESOLUTION];
  double fast_cos_table[MAX_RESOLUTION];

  struct table_filler {
    table_filler() {
      for (size_t i = 0; i < MAX_RESOLUTION; i++) {
        fast_sin_table[i] = std::sin(i * L_PI / HALF_MAX_RESOLUTION);
        fast_cos_table[i] = std::cos(i * L_PI / HALF_MAX_RESOLUTION);
      }
    }
  } table_filler_instance;

  inline double cos(double n) {
    double f = n * HALF_MAX_RESOLUTION / L_PI;
    int i1 = int(f) & MASK_MAX_RESOLUTION;
    int i2 = (i1 + 1) & MASK_MAX_RESOLUTION;
    double cos1 = fast_cos_table[i1];
    double cos2 = fast_cos_table[i2];
    return cos1 + (cos2 - cos1) * (f - i1);
  }

  inline double sin(double n) {
    double f = n * HALF_MAX_RESOLUTION / L_PI;
    int i1 = int(f) & MASK_MAX_RESOLUTION;
    int i2 = (i1 + 1) & MASK_MAX_RESOLUTION;
    double sin1 = fast_sin_table[i1];
    double sin2 = fast_sin_table[i2];
    return sin1 + (sin2 - sin1) * (f - i1);
  }

  inline double tan(double n) {
    return sin(n) / cos(n);
  }

  inline double ctg(double n) {
    return cos(n) / sin(n);
  }
}
