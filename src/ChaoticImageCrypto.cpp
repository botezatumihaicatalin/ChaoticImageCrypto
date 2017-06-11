// ChaoticImageCrypto.cpp : Defines the entry point for the console application.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <iomanip>
#include <memory>
#include "pwlcm_spic_cipher.hpp"
#include "serpentine_spic_cipher.hpp"
#include "iesidcm_cipher.hpp"

template <typename T>
void print_array(T* array, size_t size) {
  std::cout << "The array: ";
  for (size_t i = 0; i < size - 1; i ++) {
    std::cout << +array[i] << ", ";
  }
  std::cout << +array[size - 1] << "\n";
}

template <typename T>
bool arrays_are_equal(T* array1, T* array2, size_t size) {
  bool result = true;
  for (size_t i = 0; i < size; i++) {
    result &= array1[i] == array2[i];
  }
  return result;
}

int main() {
  uint32_t size = 800 * 600;
  const size_t channels = 4;

  std::unique_ptr<uint8_t[]> pixels(new uint8_t[size * channels]);
  for (uint32_t i = 0; i < size; i++) {
    std::fill(&pixels[i * channels], &pixels[i * channels + channels], i);
  }

  clock_t start, end;
  
  //pwlcm_spic<channels> encryptor;
  //encryptor.init_key(pwlcm_spic_key(dvec2(0.1567, 0.3219), dvec2(0.4567, 0.1111),
                                   // 0.2, 0.3, 2017, 2016, 123456));
  iesidcm_cipher<channels> encryptor;
  

  start = clock();
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
