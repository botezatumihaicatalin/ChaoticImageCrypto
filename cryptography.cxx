#include <nan.h>
#include "src/pwlcm_spic.hpp"
#include "src/pwlcm_spic_key.hpp"
#include <iostream>

pwlcm_spic<4> image_cipher;

NAN_METHOD(Encrypt) {
  if (info[0]->IsUint8ClampedArray()) {
    // Get typed array contents.
    v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
    Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

    // Perform the encryption.
    uint8_t* output = image_cipher.encrypt(*inputContents, inputContents.length());

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
    uint8_t* output = image_cipher.encrypt(*inputContents, inputContents.length());

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

NAN_METHOD(Decrypt) {
  if (info[0]->IsUint8ClampedArray()) {
    // Get typed array contents.
    v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
    Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

    // Perform the encryption.
    uint8_t* output = image_cipher.decrypt(*inputContents, inputContents.length());

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
    uint8_t* output = image_cipher.decrypt(*inputContents, inputContents.length());

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

NAN_MODULE_INIT(Initialize) {
  image_cipher.init_key(new pwlcm_spic_key(dvec2(0.1567, 0.3219), dvec2(0.4567, 0.1111),
                                           0.2, 0.3, 2017, 2016, 123456));
  NAN_EXPORT(target, Encrypt);
  NAN_EXPORT(target, Decrypt);
}

NODE_MODULE(addon, Initialize);