#pragma once

#include <nan.h>
#include <string>

#include "./utils.hpp"
#include "../src/iesidcm.hpp"

template <size_t spectrum>
class IesidcmCipher : public Nan::ObjectWrap {
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
  iesidcm<spectrum> image_cipher_;

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
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
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

  static NAN_METHOD(Encrypt) {
    // Unwrap the javascript object, into our c++ object.
    IesidcmCipher* obj = Nan::ObjectWrap::Unwrap<IesidcmCipher>(info.Holder());

    if (info[0]->IsUint8ClampedArray()) {
      // Get typed array contents.
      v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
      Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

      // Perform the encryption.
      uint8_t* output = obj->image_cipher_.encrypt(*inputContents, inputContents.length());

      // Create output Uint8Array.
      v8::Local<v8::ArrayBuffer> outputBuffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), inputContents.length());
      v8::Local<v8::Uint8ClampedArray> outputArray = v8::Uint8ClampedArray::New(outputBuffer, 0, inputContents.length());
      Nan::TypedArrayContents<uint8_t> outputContents(outputArray);

      // Copy the output into the Uint8Array.
      memcpy(*outputContents, output, sizeof(uint8_t) * outputContents.length());
      delete[] output;

      info.GetReturnValue().Set(outputArray);
    }
    else if (info[0]->IsUint8Array()) {
      // Get Uint8Array contents.
      v8::Local<v8::Uint8Array> inputArray = info[0].As<v8::Uint8Array>();
      Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

      // Perform the encryption.
      uint8_t* output = obj->image_cipher_.encrypt(*inputContents, inputContents.length());

      // Create output Uint8Array.
      v8::Local<v8::ArrayBuffer> outputBuffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), inputContents.length());
      v8::Local<v8::Uint8Array> outputArray = v8::Uint8Array::New(outputBuffer, 0, inputContents.length());
      Nan::TypedArrayContents<uint8_t> outputContents(outputArray);

      // Copy the output into the Uint8Array.
      memcpy(*outputContents, output, sizeof(uint8_t) * outputContents.length());
      delete[] output;

      info.GetReturnValue().Set(outputArray);
    }
    else {
      return Nan::ThrowError("First argument must be an Uint8Array or and Uint8ClampedArray");
    }
  }

  static NAN_METHOD(Decrypt) {
    // Unwrap the javascript object, into our c++ object.
    IesidcmCipher* obj = Nan::ObjectWrap::Unwrap<IesidcmCipher>(info.Holder());

    if (info[0]->IsUint8ClampedArray()) {
      // Get typed array contents.
      v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
      Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

      // Perform the encryption.
      uint8_t* output = obj->image_cipher_.decrypt(*inputContents, inputContents.length());

      // Create output Uint8Array.
      v8::Local<v8::ArrayBuffer> outputBuffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), inputContents.length());
      v8::Local<v8::Uint8ClampedArray> outputArray = v8::Uint8ClampedArray::New(outputBuffer, 0, inputContents.length());
      Nan::TypedArrayContents<uint8_t> outputContents(outputArray);

      // Copy the output into the Uint8Array.
      memcpy(*outputContents, output, sizeof(uint8_t) * outputContents.length());
      delete[] output;

      info.GetReturnValue().Set(outputArray);
    }
    else if (info[0]->IsUint8Array()) {
      // Get Uint8Array contents.
      v8::Local<v8::Uint8Array> inputArray = info[0].As<v8::Uint8Array>();
      Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

      // Perform the encryption.
      uint8_t* output = obj->image_cipher_.decrypt(*inputContents, inputContents.length());

      // Create output Uint8Array.
      v8::Local<v8::ArrayBuffer> outputBuffer = v8::ArrayBuffer::New(v8::Isolate::GetCurrent(), inputContents.length());
      v8::Local<v8::Uint8Array> outputArray = v8::Uint8Array::New(outputBuffer, 0, inputContents.length());
      Nan::TypedArrayContents<uint8_t> outputContents(outputArray);

      // Copy the output into the Uint8Array.
      memcpy(*outputContents, output, sizeof(uint8_t) * outputContents.length());
      delete[] output;

      info.GetReturnValue().Set(outputArray);
    }
    else {
      return Nan::ThrowError("First argument must be an Uint8Array or and Uint8ClampedArray");
    }
  }

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
};