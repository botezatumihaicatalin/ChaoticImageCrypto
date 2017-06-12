#pragma once

#include <nan.h>
#include <string>

#include "./base_image_cipher.hpp"
#include "./utils.hpp"

#include "../src/iesidcm_cipher.hpp"

template <size_t spectrum>
class IesidcmCipher : public BaseImageCipher<spectrum> {
public:
  static NAN_MODULE_INIT(Init) {
    const std::string class_name = "IesidcmCipher" + std::to_string(spectrum);

    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New(class_name).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "encrypt", IesidcmCipher<spectrum>::Encrypt);
    Nan::SetPrototypeMethod(tpl, "decrypt", IesidcmCipher<spectrum>::Decrypt);
    Nan::SetPrototypeMethod(tpl, "initKey", IesidcmCipher<spectrum>::InitKey);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New(class_name).ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
  }

private:
  iesidcm_cipher<spectrum> image_cipher_;

  explicit IesidcmCipher() : BaseImageCipher<spectrum>(&image_cipher_) {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      IesidcmCipher *obj = new IesidcmCipher();
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }
    else {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = { info[0] };
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(InitKey) {
    if (info.Length() < 1 || !info[0]->IsObject()) {
      return Nan::ThrowError("Invalid arguments: Need one object");
    }

    v8::Local<v8::Object> object_arg = Nan::To<v8::Object>(info[0]).ToLocalChecked();
    IesidcmCipher* wrapped_obj = Nan::ObjectWrap::Unwrap<IesidcmCipher>(info.Holder());

    try {

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