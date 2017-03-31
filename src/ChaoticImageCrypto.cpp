// ChaoticImageCrypto.cpp : Defines the entry point for the console application.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include "serpentine_generator.hpp"
#include "cmath"
#include <iomanip>

int main() {

  std::cout << std::setprecision(14);

  serpentine_generator generator(dvec2(M_PI / 4.0, 1 / 4.0), 10);
  for (size_t i = 0; i < 2017; i++) {
    const dvec2& point = generator.next();
    std::cout << point.x << " " << point.y << "\r\n";
  }
  
  std::cin.get();
  return 0;
}
