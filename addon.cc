#include <napi.h>
#include <string>
#include <iostream>
#include "local-storage.h"

class LocalStorageAdapter : public LocalStorage, public Napi::ObjectWrap<LocalStorageAdapter>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  LocalStorageAdapter(const Napi::CallbackInfo &info) : ObjectWrap(info)
  {
    std::cout << "init" << std::endl;
  }

private:
  Napi::Value Set(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
      Napi::TypeError::New(env, "Wrong number of arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    if (info[0].IsEmpty())
    {
      Napi::TypeError::New(env, "Key is required")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    if (info[1].IsEmpty())
    {
      Napi::TypeError::New(env, "Data can't be undefined")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::string key = info[0].As<Napi::String>();
    std::string data = info[1].As<Napi::String>();

    Napi::Boolean wasSuccessful = Napi::Boolean::New(env, LocalStorage::set(key, data));
    return wasSuccessful;
  }

  Napi::Value Get(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info[0].IsEmpty())
    {
      Napi::TypeError::New(env, "Key is required")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::string key = info[0].As<Napi::String>();

    Napi::String data = Napi::String::New(env, LocalStorage::get(key));
    return data;
  }

  Napi::Value Del(const Napi::CallbackInfo &info)
  {
    Napi::Env env = info.Env();

    if (info[0].IsEmpty())
    {
      Napi::TypeError::New(env, "Key is required")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::string key = info[0].As<Napi::String>();

    Napi::Boolean wasSuccessful = Napi::Boolean::New(env, LocalStorage::del(key));
    return wasSuccessful;
  }
};

Napi::Object LocalStorageAdapter::Init(Napi::Env env, Napi::Object exports)
{

  // This method is used to hook the accessor and method callbacks
  Napi::Function func = DefineClass(env, "LocalStorage", {LocalStorageAdapter::InstanceMethod("get", &LocalStorageAdapter::Get), LocalStorageAdapter::InstanceMethod("set", &LocalStorageAdapter::Set), LocalStorageAdapter::InstanceMethod("remove", &LocalStorageAdapter::Del)});

  exports.Set("LocalStorage", func);

  return exports;
}

// Initialize native add-on
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  LocalStorageAdapter::Init(env, exports);
  return exports;
}

NODE_API_MODULE(addon, Init)
