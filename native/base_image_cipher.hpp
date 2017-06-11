#pragma once

#include <nan.h> 
#include "../src/image_cipher_base.hpp"

template <size_t spectrum>
class BaseImageCipher : public Nan::ObjectWrap {

 private:
   image_cipher_base<spectrum>* image_cipher_;

 protected:
 	BaseImageCipher(image_cipher_base<spectrum>* image_cipher): image_cipher_(image_cipher) { };
 	virtual ~BaseImageCipher() { };

  static NAN_METHOD(Encrypt) {
    // Unwrap the javascript object, into our c++ object.
    BaseImageCipher* obj = Nan::ObjectWrap::Unwrap<BaseImageCipher>(info.Holder());
    
    if (info[0]->IsUint8ClampedArray()) {
	    // Get typed array contents.
	    v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
	    Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

	    // Perform the encryption.
	    uint8_t* output = obj->image_cipher_->encrypt(*inputContents, inputContents.length());

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
	    uint8_t* output = obj->image_cipher_->encrypt(*inputContents, inputContents.length());

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
    BaseImageCipher* obj = Nan::ObjectWrap::Unwrap<BaseImageCipher>(info.Holder());

    if (info[0]->IsUint8ClampedArray()) {
	    // Get typed array contents.
	    v8::Local<v8::Uint8ClampedArray> inputArray = info[0].As<v8::Uint8ClampedArray>();
	    Nan::TypedArrayContents<uint8_t> inputContents(inputArray);

	    // Perform the encryption.
	    uint8_t* output = obj->image_cipher_->decrypt(*inputContents, inputContents.length());

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
	    uint8_t* output = obj->image_cipher_->decrypt(*inputContents, inputContents.length());

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
};