#pragma once

#include <nan.h>
#include "base_spic.hpp"
#include "../src/pwlcm_spic.hpp"
#include "../src/pwlcm_spic_key.hpp"

class PwlcmSpicCipher : public BaseSpicCipher {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PwlcmSpicCipher").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "encrypt", Encrypt);
    Nan::SetPrototypeMethod(tpl, "decrypt", Decrypt);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("PwlcmSpicCipher").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
  pwlcm_spic<4> image_cipher_;

  explicit PwlcmSpicCipher(): BaseSpicCipher(&image_cipher_) {
    image_cipher_.init_key(pwlcm_spic_key(dvec2(0.1567, 0.3219), dvec2(0.4567, 0.1111),
                                                0.2, 0.3, 2017, 2016, 123456));
  }
  virtual ~PwlcmSpicCipher() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      PwlcmSpicCipher *obj = new PwlcmSpicCipher();
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } 
    else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
};