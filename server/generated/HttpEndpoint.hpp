/* Autogenerated with Kurento Idl */

#ifndef __HTTP_ENDPOINT_HPP__
#define __HTTP_ENDPOINT_HPP__

#include <jsoncpp/json/json.h>
#include <JsonRpcException.hpp>
#include <ObjectRegistrar.hpp>
#include <memory>
#include <vector>
#include "SessionEndpoint.hpp"

namespace kurento {


class HttpEndpoint : public virtual SessionEndpoint {

public:

  HttpEndpoint () {};
  virtual ~HttpEndpoint () {};

  virtual std::string getUrl () {throw "Not implemented";};

  virtual std::string connect(const std::string &eventType, std::shared_ptr<EventHandler> handler);

  class Factory : public virtual kurento::Factory
  {
  public:
    Factory () {};

    virtual std::shared_ptr<MediaObject> createObject (const Json::Value
        &params);

    virtual std::string getName () {
      return "HttpEndpoint";
    };

  private:

    std::shared_ptr<MediaObject> createObject (int garbagePeriod);

    class StaticConstructor
    {
    public:
      StaticConstructor();
    };

    static StaticConstructor staticConstructor;

  };

  class Invoker : public virtual SessionEndpoint::Invoker
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

#endif /*  __HTTP_ENDPOINT_HPP__ */
