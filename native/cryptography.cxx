#include <nan.h>
#include "pwlcm_spic.hpp"
#include "serpentine_spic.hpp"
#include "serpentine2_spic.hpp"
#include "iesidcm.hpp"

void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
  PwlcmSpicCipher<4>::Init(target); 
  SerpentineSpicCipher<4>::Init(target);
  Serpentine2SpicCipher<4>::Init(target);
  IesidcmCipher<4>::Init(target);

  PwlcmSpicCipher<3>::Init(target); 
  SerpentineSpicCipher<3>::Init(target);
  Serpentine2SpicCipher<3>::Init(target);
  IesidcmCipher<3>::Init(target);
}

NODE_MODULE(cryptography, Initialize);