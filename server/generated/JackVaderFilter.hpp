/* Autogenerated with Kurento Idl */

#ifndef __JACK_VADER_FILTER_HPP__
#define __JACK_VADER_FILTER_HPP__

#include <jsoncpp/json/json.h>
#include <JsonRpcException.hpp>
#include <ObjectRegistrar.hpp>
#include <memory>
#include <vector>
#include "Filter.hpp"

namespace kurento {

class MediaPipeline;

class JackVaderFilter : public virtual Filter {

public:

  JackVaderFilter () {};
  virtual ~JackVaderFilter () {};

  virtual std::string connect(const std::string &eventType, std::shared_ptr<EventHandler> handler);

  class Factory : public virtual kurento::Factory
  {
  public:
    Factory () {};

    virtual std::shared_ptr<MediaObject> createObject (const Json::Value
        &params);

    virtual std::string getName () {
      return "JackVaderFilter";
    };

  private:

    std::shared_ptr<MediaObject> createObject (std::shared_ptr<MediaPipeline> mediaPipeline, int garbagePeriod);

    class StaticConstructor
    {
    public:
      StaticConstructor();
    };

    static StaticConstructor staticConstructor;

  };

  class Invoker : public virtual Filter::Invoker
  {
  public:
    Invoker() {};
    virtual void invoke (std::shared_ptr<MediaObject> obj,
        const std::string &methodName, const Json::Value &params,
        Json::Value &response);
  };

  virtual MediaObject::Invoker &getInvoker() {
    return invoker;
  }

  private:
    Invoker invoker;

};

} /* kurento */

#endif /*  __JACK_VADER_FILTER_HPP__ */
