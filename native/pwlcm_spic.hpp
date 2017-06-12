#pragma once

#include <nan.h>
#include <string>

#include "./base_image_cipher.hpp"
#include "./utils.hpp"

#include "../src/pwlcm_spic_cipher.hpp"
#include "../src/pwlcm_spic_key.hpp"

template <size_t spectrum>
class PwlcmSpicCipher : public BaseImageCipher<spectrum> {
public:
  static NAN_MODULE_INIT(Init) {
    const std::string class_name = "PwlcmSpicCipher" + std::to_string(spectrum);

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New(class_name).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "encrypt", PwlcmSpicCipher<spectrum>::Encrypt);
    Nan::SetPrototypeMethod(tpl, "decrypt", PwlcmSpicCipher<spectrum>::Decrypt);
    Nan::SetPrototypeMethod(tpl, "initKey", PwlcmSpicCipher<spectrum>::InitKey);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New(class_name).ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

private:
  pwlcm_spic_cipher<spectrum> image_cipher_;

  explicit PwlcmSpicCipher(): BaseImageCipher<spectrum>(&image_cipher_) {}

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
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(InitKey) {
    if (info.Length() < 1 || !info[0]->IsObject()) {
      return Nan::ThrowError("Invalid arguments: Need one object");
    }
    double x1 = 0, y1 = 0, r1 = 0, x2 = 0, y2 = 0, r2 = 0;
    uint32_t m1 = 0, m2 = 0, iv = 0;

    v8::Local<v8::Object> object_arg = Nan::To<v8::Object>(info[0]).ToLocalChecked();
    PwlcmSpicCipher* wrapped_obj = Nan::ObjectWrap::Unwrap<PwlcmSpicCipher>(info.Holder());

    try {
      // First generator params
      x1 = GetNumberValue(object_arg, "x1"), y1 = GetNumberValue(object_arg, "y1");
      r1 = GetNumberValue(object_arg, "r1"), m1 = GetUint32Value(object_arg, "m1");

      // Second generator params
      x2 = GetNumberValue(object_arg, "x2"), y2 = GetNumberValue(object_arg, "y2");
      r2 = GetNumberValue(object_arg, "r2"), m2 = GetUint32Value(object_arg, "m2");

      // Initialization value, used to spice up things.
      iv = GetUint32Value(object_arg, "iv");

      pwlcm_spic_key cipher_key(dvec2(x1, y1), dvec2(x2, y2), r1, r2, m1, m2, iv);
      wrapped_obj->image_cipher_.init_key(cipher_key);
    }
    catch (std::exception& ex) {
      return Nan::ThrowError(ex.what());
    }

    info.GetReturnValue().Set(info.Holder());
  }

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
};