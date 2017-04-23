#include <nan.h>
#include "pwlcm_spic.hpp"
#include "serpentine_spic.hpp"

void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
  PwlcmSpicCipher::Init(target); SerpentineSpicCipher::Init(target);
}

NODE_MODULE(cryptography, Initialize);