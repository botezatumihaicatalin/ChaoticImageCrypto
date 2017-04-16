// ChaoticImageCrypto.cpp : Defines the entry point for the console application.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <iomanip>
#include <memory>
#include "pwlcm_spic.hpp"
#include "serpentine_spic.hpp"

template<typename T>
void print_array(T* array, size_t size) {
  std::cout << "The array: ";
  for (size_t i = 0; i < size - 1; i ++) {
    std::cout << +array[i] << ", ";
  }
  std::cout << +array[size - 1] << "\n";
}

template<typename T>
bool arrays_are_equal(T* array1, T* array2, size_t size) {
  bool result = true;
  for (size_t i = 0; i < size; i++) {
    result &= array1[i] == array2[i];
  }
  return result;
}

int main() {
  uint32_t size = 800 * 600;
  size_t channels = 4;

  std::unique_ptr<uint8_t[]> pixels(new uint8_t[size * channels]);
  for (uint32_t i = 0; i < size; i++) {
    std::fill(&pixels[i * 4], &pixels[i * 4 + 4], i);
  }
  
  clock_t start, end;
  
  start = clock();
  serpentine_spic<4> encryptor;
  std::unique_ptr<uint8_t[]> encrypted(encryptor.encrypt(pixels.get(), size * channels));
  end = clock();

  std::cout << "Encryption: " << end - start << "\n";

  start = clock();
  std::unique_ptr<uint8_t[]> decrypted(encryptor.decrypt(encrypted.get(), size * channels));
  end = clock();

  std::cout << "Decryption: " << end - start << "\n";

  if (size < 32) {
    print_array(pixels.get(), size * channels);
    print_array(encrypted.get(), size * channels);
    print_array(decrypted.get(), size * channels);
  }
  
  if (arrays_are_equal(pixels.get(), decrypted.get(), size * channels)) {
    std::cout << "The decyption is the same as encryption \n";
  }

  std::cin.get();

  return 0;
}
