#pragma once

#include <nan.h>
#include "base_spic.hpp"
#include "../src/serpentine_spic.hpp"
#include "../src/serpentine_spic_key.hpp"

class SerpentineSpicCipher : public BaseSpicCipher {
 public:
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("SerpentineSpicCipher").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "encrypt", Encrypt);
    Nan::SetPrototypeMethod(tpl, "decrypt", Decrypt);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("SerpentineSpicCipher").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:
  serpentine_spic<4> image_cipher_;

  explicit SerpentineSpicCipher(): BaseSpicCipher(&image_cipher_) {
    image_cipher_.init_key(new serpentine_spic_key(dvec2(M_PI / 4.0, 1.0 / 4.0), 
                                             dvec2(-M_PI / 4.0, -1 / 20.0), 
                                             10, 20, 2017, 2016, 123456));
  }
  virtual ~SerpentineSpicCipher() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      SerpentineSpicCipher *obj = new SerpentineSpicCipher();
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } 
    else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = { info[0] };
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
};