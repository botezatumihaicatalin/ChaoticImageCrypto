#pragma once

#include <nan.h>
#include <stdexcept>

v8::Local<v8::Number> GetLocalNumber(const v8::Local<v8::Object>& object, const std::string& field) {
  v8::Local<v8::Value> value_local = object->Get(Nan::New(field).ToLocalChecked());
  if (!value_local->IsNumber()) {
    throw std::runtime_error(std::string("Object must have key ") + field + std::string(" = Number"));
  }
  return Nan::To<v8::Number>(value_local).ToLocalChecked();
}

double GetNumberValue(const v8::Local<v8::Object>& object, const std::string& field) {
  return GetLocalNumber(object, field)->NumberValue();
}

uint32_t GetUint32Value(const v8::Local<v8::Object>& object, const std::string& field) {
  return GetLocalNumber(object, field)->Uint32Value();
}